#pragma once

#include "CoreMinimal.h"
#include "AppComponent.h"
#include "EnvironmentCommand.h"
#include "ICommunicationChannel.h"
#include "SocketListener.h"

THIRD_PARTY_INCLUDES_START
#include "oatpp/network/Server.hpp"
THIRD_PARTY_INCLUDES_END

class FCommandListener : public ICommunicationChannel
{
public:
	FCommandListener();
	
	virtual void Start(uint16 Port) override;

	virtual void Stop() override;

	virtual void RequestAction(const FAgentState& State) override;

	virtual FAgentAction WaitForAction(const FString& AgentId) override;

	TQueue<std::shared_ptr<FEnvironmentCommand>, EQueueMode::Mpsc> PendingCommands;

	//TMap<FString, std::shared_ptr<FEnvironmentCommand>> PendingCommands;

	TMap<FString, TTuple<FAgentState, TArray<float>>> PendingActionRequests;

	TQueue<FString, EQueueMode::Spsc> ActionRequestQueue;

	TMap<FString, FAgentAction> ReceivedActions;

private:
	std::shared_ptr<oatpp::network::Server> OatppServer;

	std::shared_ptr<ConnectionInstanceListener> ConnectionListener;

	std::shared_ptr<oatpp::data::mapping::ObjectMapper> Mapper;

	std::thread ServerThread;

	std::atomic_bool ShouldListen;

	bool bShouldWaitForAction;

	void SendPendingRequests();

	void ProcessClientCommand();

	bool PollClientCommand(const FTimespan& Timeout, std::shared_ptr<FEnvironmentCommand>& OutCommand);

	bool WaitClientConnection(const FTimespan& Timeout);
};
