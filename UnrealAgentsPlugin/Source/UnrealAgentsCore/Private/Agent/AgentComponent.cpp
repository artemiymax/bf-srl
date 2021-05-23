#include "Agent/AgentComponent.h"
#include "Agent/AgentState.h"
#include "Behavior/BehaviorComponent.h"
#include "Sensor/ISensor.h"
#include "Sensor/ISensorComponent.h"
#include "EnvironmentDirector.h"
#include "Behavior/NoopPolicy.h"

UAgentComponent::UAgentComponent()
{
	BehaviorComponent = CreateDefaultSubobject<UBehaviorComponent>(TEXT("BehaviorComponent"));
}

void UAgentComponent::BeginPlay()
{
	Super::BeginPlay();

	ConfigureBehavior();
	ConfigureSensors();

	State.AgentId = GetOwner()->GetName();

	ResetDelegateHandle = FEnvironmentDirector::Get().OnResetAction().AddUObject(this, &UAgentComponent::Reset);
	StepDelegateHandle = FEnvironmentDirector::Get().OnStepAction().AddUObject(this, &UAgentComponent::Step);
	ObserveDelegateHandle = FEnvironmentDirector::Get().OnObserveAction().AddUObject(this, &UAgentComponent::SendObserveRequest);
	DecideDelegateHandle = FEnvironmentDirector::Get().OnDecideAction().AddUObject(this, &UAgentComponent::SendDecideRequest);
	ActDelegateHandle = FEnvironmentDirector::Get().OnActAction().AddUObject(this, &UAgentComponent::SendActRequest);
}

void UAgentComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	FEnvironmentDirector::Get().OnResetAction().Remove(ResetDelegateHandle);
	FEnvironmentDirector::Get().OnStepAction().Remove(StepDelegateHandle);
	FEnvironmentDirector::Get().OnObserveAction().Remove(ObserveDelegateHandle);
	FEnvironmentDirector::Get().OnDecideAction().Remove(DecideDelegateHandle);
	FEnvironmentDirector::Get().OnActAction().Remove(ActDelegateHandle);
}

void UAgentComponent::OnBeginEpisode() {}

void UAgentComponent::OnObserve() {}

void UAgentComponent::OnAct(const FAgentAction& Action) {}

void UAgentComponent::EndEpisode()
{
	EpisodeCount++;

	State.EpisodeReward = EpisodeReward;
	State.StepReward = StepReward;
	State.bIsDone = true;

	Policy->UpdateState(State);

	UpdateEpisodeMetrics();

	Reset();
}

void UAgentComponent::AddReward(const float Reward)
{
	StepReward += Reward;
	EpisodeReward += Reward;
}

void UAgentComponent::SetReward(const float Reward)
{
	EpisodeReward += (StepReward - Reward);
	StepReward = Reward;
}

void UAgentComponent::Reset()
{
	EpisodeStepCount = 0;
	StepReward = 0.f;
	EpisodeReward = 0.f;

	for (ISensor* Sensor : Sensors)
	{
		Sensor->Reset();
	}

	OnBeginEpisode();
}

void UAgentComponent::Step()
{
	EpisodeStepCount++;
	TotalStepCount++;
}

void UAgentComponent::UpdateEpisodeMetrics()
{
	FString AgentName = GetOwner()->GetName();
	FString LengthMetric = FString::Format(TEXT("agent/{0}/episode_length"), { AgentName });
	FString RewardMetric = FString::Format(TEXT("agent/{0}/episode_reward"), { AgentName });

	IMetricsWriter* MetricsWriter = FEnvironmentDirector::Get().GetMetricsWriter();
	MetricsWriter->AddMetric(RewardMetric, EpisodeReward, Mean);
	MetricsWriter->AddMetric(LengthMetric, EpisodeStepCount, Mean);
	MetricsWriter->AddMetric(FString("environment/episode_reward"), EpisodeReward, Mean);
}

void UAgentComponent::SendObserveRequest()
{
	for (ISensor* Sensor : Sensors)
	{
		Sensor->Update();
	}
	
	OnObserve();

	State.EpisodeReward = EpisodeReward;
	State.StepReward = StepReward;
	State.bIsDone = false;

	Policy->UpdateState(State);

	StepReward = 0.f;
}

void UAgentComponent::SendDecideRequest()
{
	CurrentAction = Policy->DecideAction();
}

void UAgentComponent::SendActRequest()
{
	OnAct(CurrentAction);
}

void UAgentComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAgentComponent::ConfigureBehavior()
{
	if (!BehaviorComponent->PolicyType)
	{
		Policy = new FNoopPolicy();
	}
	else
	{
		Policy = Cast<IPolicy>(BehaviorComponent->PolicyType);
		Policy->Initialize(FActionSpace(BehaviorComponent->ActionType, BehaviorComponent->ActionShape));
	}
}

void UAgentComponent::ConfigureSensors()
{
	for (UActorComponent* Component : GetOwner()->GetComponentsByInterface(USensorComponent::StaticClass()))
	{
		ISensorComponent* SensorComponent = Cast<ISensorComponent>(Component);
		SensorComponent->InitializeSensor();

		Sensors.Add(SensorComponent->GetSensor());
	}

	State.Sensors = &Sensors;
}
