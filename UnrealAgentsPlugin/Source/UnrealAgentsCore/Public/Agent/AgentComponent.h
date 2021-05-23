#pragma once

#include "CoreMinimal.h"
#include "AgentState.h"
#include "AgentAction.h"
#include "Behavior/IPolicy.h"
#include "Behavior/BehaviorComponent.h"
#include "Components/ActorComponent.h"
#include "AgentComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNREALAGENTSCORE_API UAgentComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAgentComponent();

	UPROPERTY(Category = Agent, VisibleInstanceOnly)
	UBehaviorComponent* BehaviorComponent;

	uint32 GetEpisodeStepCount() const { return EpisodeStepCount; }

	FAgentState GetState() const { return State; }

	FAgentAction GetCurrentAction() const { return CurrentAction; }

	virtual void OnBeginEpisode();

	virtual void OnObserve();

	virtual void OnAct(const FAgentAction& Action);

	void EndEpisode();

	void AddReward(float Reward);

	void SetReward(float Reward);

	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	void BeginPlay() override;

	void EndPlay(EEndPlayReason::Type EndPlayReason) override;

private:
	bool bIsReset;

	uint32 EpisodeCount;

	uint32 EpisodeStepCount;

	uint32 TotalStepCount;

	float StepReward;

	float EpisodeReward;

	IPolicy* Policy;

	FAgentState State;

	FAgentAction CurrentAction;

	TArray<ISensor*> Sensors;

	FDelegateHandle ResetDelegateHandle;

	FDelegateHandle StepDelegateHandle;

	FDelegateHandle ObserveDelegateHandle;

	FDelegateHandle DecideDelegateHandle;

	FDelegateHandle ActDelegateHandle;

	void Reset();

	void Step();

	void UpdateEpisodeMetrics();

	void SendObserveRequest();

	void SendDecideRequest();

	void SendActRequest();

	void ConfigureBehavior();

	void ConfigureSensors();
};
