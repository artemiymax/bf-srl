#include "EnvironmentDirector.h"

#include "CommandListener.h"
#include "EnvironmentProfiler.h"
#include "TensorboardMetricsLogger.h"
#include "UnrealAgentsSubsystem.h"
#include "Kismet/GameplayStatics.h"

FEnvironmentDirector::FEnvironmentDirector() :
	bIsInitialized(false),
	bIsFirstStep(false),
	EpisodeCount(0),
	EpisodeStepCount(0),
	TotalStepCount(0),
	EnvironmentSettings(nullptr),
	MetricsWriter(nullptr),
	RemoteChannel(nullptr)
{
	Ticker = NewObject<UEnvironmentTicker>();
	Ticker->AddToRoot();
}

FEnvironmentDirector& FEnvironmentDirector::Get()
{
	UGameInstance* GameInstance = FEnvironmentHelper::GetWorld()->GetGameInstance();
	UUnrealAgentsSubsystem* UnrealAgentsSubsystem = GameInstance->GetSubsystem<UUnrealAgentsSubsystem>();

	return UnrealAgentsSubsystem->GetEnvironmentDirector();
}

void FEnvironmentDirector::Initialize(UEnvironmentSettings* Settings)
{
	if (IsInitialized())
	{
		return;
	}

	EnvironmentSettings = Settings;
	FApp::SetUseFixedTimeStep(EnvironmentSettings->bUseFixedTimeStep);
	FApp::SetFixedDeltaTime(EnvironmentSettings->FixedTimeStep);

	if (Settings->bRecordMetrics)
	{
		MetricsWriter = new FMetricsWriter(Settings->FlushInterval);
		if (Settings->DefaultLogger == EMetricsLoggerType::Tensorboard)
		{
			MetricsWriter->RegisterLogger("TensorboardLogger", new FTensorboardMetricsLogger());
		}
	}
	else
	{
		MetricsWriter = new FNoopMetricsWriter();
	}

	if (Settings->bUseRemoteControl)
	{
		RemoteChannel = new FCommandListener();
		RemoteChannel->Start(Settings->ProxyPort);
	}

	bIsInitialized = true;
}

void FEnvironmentDirector::Reset()
{
	EpisodeCount++;
	EpisodeStepCount = 0;

	OnResetAction().Broadcast();
}

void FEnvironmentDirector::Step()
{
	if (!bIsFirstStep)
	{
		bIsFirstStep = true;
		OnResetAction().Broadcast();
	}

	EpisodeStepCount++;
	TotalStepCount++;

	StepGlobalAgent();
	StepWorldAgents();

	UpdateMetrics();
}

void FEnvironmentDirector::StepGlobalAgent()
{
	OnGlobalStepAction().Broadcast();
	OnGlobalObserveAction().Broadcast();
	OnGlobalDecideAction().Broadcast();
	OnGlobalActAction().Broadcast();
}

void FEnvironmentDirector::StepWorldAgents()
{
	OnStepAction().Broadcast();
	OnObserveAction().Broadcast();
	OnDecideAction().Broadcast();
	OnActAction().Broadcast();
}

void FEnvironmentDirector::UpdateMetrics()
{
	FEnvironmentProfiler::UpdateMetrics();
}

FEnvironmentDirector::~FEnvironmentDirector()
{
	if (RemoteChannel)
	{
		RemoteChannel->Stop();
		delete RemoteChannel;
	}

	delete MetricsWriter;
}