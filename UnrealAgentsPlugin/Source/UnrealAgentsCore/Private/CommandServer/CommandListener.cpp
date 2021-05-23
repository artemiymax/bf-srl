#include "CommandListener.h"
#include "AgentStep.h"
#include "AppComponent.h"
#include "DtoConverter.h"
#include "EnvironmentDirector.h"
#include "SocketController.h"
#include "Async/Async.h"

THIRD_PARTY_INCLUDES_START
#include "oatpp/network/Server.hpp"
#include "oatpp-websocket/Connector.hpp"
THIRD_PARTY_INCLUDES_END

FCommandListener::FCommandListener() : bShouldWaitForAction(false)
{
}

void FCommandListener::Start(const uint16 Port)
{
	ServerThread = std::thread([this, Port]
		{
			ShouldListen.store(true);

			oatpp::base::Environment::init();

			AppComponent Components(Port);

			auto Router = Components.HTTPRouter.getObject();
			auto socketController = std::make_shared<FSocketController>();
			socketController->addEndpointsToRouter(Router);

			auto ConnectionHandler = Components.ServerConnectionHandler.getObject();
			auto ConnectionProvider = Components.ServerConnectionProvider.getObject();
			OatppServer = oatpp::network::Server::createShared(ConnectionProvider, ConnectionHandler);
			ConnectionListener = std::reinterpret_pointer_cast<ConnectionInstanceListener>(Components.SocketInstanceListener.getObject());
			ConnectionListener->CommandQueue = &PendingCommands;
			Mapper = Components.ApiObjectMapper.getObject();

			std::function<bool()> RunCondition = [this]() { return ShouldListen.load(); };

			OatppServer->run(RunCondition);

			ConnectionProvider->stop();
			ConnectionHandler->stop();
		});

	if (!WaitClientConnection(FTimespan::FromSeconds(30)))
	{
		GLog->Log(TEXT("Failed acquire client connection"));
		FPlatformMisc::RequestExit(false);
	}
}

void FCommandListener::Stop()
{
	ShouldListen.store(false);
	if (ServerThread.joinable())
	{
		ServerThread.join();
	}
}

void FCommandListener::RequestAction(const FAgentState& State)
{
	TTuple<FAgentState, TArray<float>>* CurrentState = PendingActionRequests.Find(State.AgentId);
	if (CurrentState != nullptr && CurrentState->Key.bIsDone)
	{
		return;
	}

	TTuple<FAgentState, TArray<float>> NewState(State, TArray<float>());
	for (ISensor* Sensor : *State.Sensors)
	{
		Sensor->Read(NewState.Value);
	}

	ActionRequestQueue.Enqueue(State.AgentId);
	PendingActionRequests.Add(State.AgentId, NewState);

	bShouldWaitForAction = true;
}

FAgentAction FCommandListener::WaitForAction(const FString& AgentId)
{
	if (bShouldWaitForAction)
	{
		SendPendingRequests();

		ProcessClientCommand();

		bShouldWaitForAction = false;
	}

	return ReceivedActions.Contains(AgentId)
		? ReceivedActions[AgentId]
		: FAgentAction();
}

void FCommandListener::SendPendingRequests()
{
	oatpp::List<oatpp::data::mapping::type::DTOWrapper<FAgentStepDto>> Request = oatpp::List<oatpp::data::mapping::type::DTOWrapper<FAgentStepDto>>::createShared();

	FString PendingAgentId;
	while (ActionRequestQueue.Dequeue(PendingAgentId))
	{
		TTuple<FAgentState, TArray<float>>* PendingState = PendingActionRequests.Find(PendingAgentId);
		if (PendingState == nullptr)
		{
			continue;
		}

		oatpp::data::mapping::type::DTOWrapper<FAgentStepDto> AgentStepDto = FDtoConverter::ToAgentStepDto(PendingState);
		Request->push_back(AgentStepDto);

		PendingActionRequests.Remove(PendingAgentId);
	}

	if (!ConnectionListener->Client)
	{
		std::shared_ptr<FEnvironmentCommand> Command;
		if (!PollClientCommand(FTimespan::FromSeconds(30), Command))
		{
			GLog->Log(TEXT("Failed to Poll Client"));
			return;
		}
	}

	ConnectionListener->Client->sendOneFrameText(Mapper->writeToString(Request));
}

void FCommandListener::ProcessClientCommand()
{
	std::shared_ptr<FEnvironmentCommand> Command;
	if (!PollClientCommand(FTimespan::FromSeconds(30), Command))
	{
		GLog->Log(TEXT("Failed to Poll Client"));
		return;
	}

	if (Command->Type == "step")
	{
		ReceivedActions = FDtoConverter::ToAgentActionMap(oatpp::String(TCHAR_TO_UTF8(*Command->Data)), Mapper);
	}
	else if (Command->Type == "reset")
	{
		PendingCommands.Empty();
		ActionRequestQueue.Empty();
		PendingActionRequests.Empty();

		FEnvironmentDirector::Get().Reset();
	}
	else if (Command->Type == "close")
	{
		FPlatformMisc::RequestExit(true);
	}
}

bool FCommandListener::PollClientCommand(const FTimespan& Timeout, std::shared_ptr<FEnvironmentCommand>& OutCommand)
{
	bool bIsTimeout = false;
	FDateTime StartTime = FDateTime::UtcNow();
	while (!PendingCommands.Dequeue(OutCommand))
	{
		if (FDateTime::UtcNow() - StartTime > Timeout)
		{
			bIsTimeout = true;
			break;
		}
	}

	return !bIsTimeout;
}

bool FCommandListener::WaitClientConnection(const FTimespan& Timeout)
{
	bool bIsTimeout = false;
	FDateTime StartTime = FDateTime::UtcNow();
	while (!ConnectionListener || !ConnectionListener->Client)
	{
		if (FDateTime::UtcNow() - StartTime > Timeout)
		{
			bIsTimeout = true;
			break;
		}
	}

	return !bIsTimeout;
}
