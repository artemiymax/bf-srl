#include "UnrealAgentsCore.h"

#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "EnvironmentDirector.h"
#include "EnvironmentHelper.h"

#define LOCTEXT_NAMESPACE "FUnrealAgentsCoreModule"

void FUnrealAgentsCoreModule::StartupModule()
{
	FConsoleCommandWithArgsDelegate AgentCommandDelegate;
	AgentCommandDelegate.BindRaw(this, &FUnrealAgentsCoreModule::AgentCommand);

	IConsoleObject* VAgentCmd = IConsoleManager::Get().RegisterConsoleCommand(
		TEXT("vagent"),
		TEXT("RL Framework commands"),
		AgentCommandDelegate);
}

void FUnrealAgentsCoreModule::ShutdownModule()
{
	GLog->Log("UnrealAgents shutdown");
}

void FUnrealAgentsCoreModule::AgentCommand(const TArray<FString>& Args)
{
	if (Args.Num() < 1)
	{
		return;
	}

	if (Args[0] == "start-tick")
	{
		FEnvironmentDirector::Get().GetSettings()->bUseAutoStep = true;
	}
	else if (Args[0] == "stop-tick")
	{
		FEnvironmentDirector::Get().GetSettings()->bUseAutoStep = false;
	}
	else if (Args[0] == "reset")
	{
		FEnvironmentDirector::Get().Reset();
	}
	else if (Args[0] == "set-dilation")
	{
		UWorld* World = FEnvironmentHelper::GetWorld();

		const float Dilation = Args.Num() > 1 ? FCString::Atof(*Args[1]) : 1.f;
		UGameplayStatics::SetGlobalTimeDilation(World, Dilation);
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FUnrealAgentsCoreModule, UnrealAgentsCore)