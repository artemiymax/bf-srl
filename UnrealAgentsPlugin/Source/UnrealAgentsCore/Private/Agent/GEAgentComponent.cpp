#include "Agent/GEAgentComponent.h"
#include "EnvironmentDirector.h"
#include "EnvironmentHelper.h"
#include "Agent/AgentComponent.h"
#include "Behavior/NoopPolicy.h"
#include "Kismet/GameplayStatics.h"

UGEAgentComponent::UGEAgentComponent()
{
}

void UGEAgentComponent::BeginPlay()
{
	Super::BeginPlay();

	if (PolicyType)
	{
		Policy = Cast<IGlobalPolicy>(PolicyType);
		Policy->Initialize();
	}
	else
	{
		Policy = new FNoopGlobalPolicy();
	}

	ResetDelegateHandle = FEnvironmentDirector::Get().OnGlobalResetAction().AddUObject(this, &UGEAgentComponent::Reset);
	StepDelegateHandle = FEnvironmentDirector::Get().OnGlobalStepAction().AddUObject(this, &UGEAgentComponent::Step);
	ObserveDelegateHandle = FEnvironmentDirector::Get().OnGlobalObserveAction().AddUObject(this, &UGEAgentComponent::SendObserveRequest);
	DecideDelegateHandle = FEnvironmentDirector::Get().OnGlobalDecideAction().AddUObject(this, &UGEAgentComponent::SendDecideRequest);
	ActDelegateHandle = FEnvironmentDirector::Get().OnGlobalActAction().AddUObject(this, &UGEAgentComponent::SendActRequest);
}

void UGEAgentComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	FEnvironmentDirector::Get().OnGlobalResetAction().Remove(ResetDelegateHandle);
	FEnvironmentDirector::Get().OnGlobalStepAction().Remove(StepDelegateHandle);
	FEnvironmentDirector::Get().OnGlobalObserveAction().Remove(ObserveDelegateHandle);
	FEnvironmentDirector::Get().OnGlobalDecideAction().Remove(DecideDelegateHandle);
	FEnvironmentDirector::Get().OnGlobalActAction().Remove(ActDelegateHandle);
}

void UGEAgentComponent::OnObserve()
{
}

void UGEAgentComponent::OnAct(const FAgentAction& Action)
{
}

void UGEAgentComponent::Reset()
{
	TotalStepCount = 0;

	State.AgentStates.Reset();
	State.ActionMap.Reset();
}

void UGEAgentComponent::Step()
{
	TotalStepCount++;
}

void UGEAgentComponent::SendObserveRequest()
{
	TArray<UAgentComponent*> EnvironmentAgents = FEnvironmentHelper::GetComponentsByClass<UAgentComponent>();

	for (const UAgentComponent* Agent : EnvironmentAgents)
	{
		FName AgentName(Agent->GetName());
		State.AgentStates.Add(AgentName, Agent->GetState());
		State.ActionMap.Add(AgentName, Agent->GetCurrentAction());
	}

	OnObserve();

	Policy->UpdateState(State);
}

void UGEAgentComponent::SendDecideRequest()
{
	CurrentAction = Policy->DecideAction();
}

void UGEAgentComponent::SendActRequest()
{
	OnAct(CurrentAction);
}