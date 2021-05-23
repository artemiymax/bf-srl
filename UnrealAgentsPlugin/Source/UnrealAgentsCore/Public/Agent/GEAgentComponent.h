#pragma once

#include "CoreMinimal.h"
#include "AgentAction.h"
#include "Behavior/IGlobalPolicy.h"
#include "Components/ActorComponent.h"
#include "GEAgentComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNREALAGENTSCORE_API UGEAgentComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UGEAgentComponent();

	UPROPERTY(Category = "GEAgent|GlobalPolicy", EditInstanceOnly, Instanced)
	UGlobalPolicyBase* PolicyType;

	virtual void OnObserve();

	virtual void OnAct(const FAgentAction& Action);

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

private:
	uint32 TotalStepCount;

	IGlobalPolicy* Policy;

	FGEAgentState State;

	FAgentAction CurrentAction;

	FDelegateHandle ResetDelegateHandle;

	FDelegateHandle StepDelegateHandle;

	FDelegateHandle ObserveDelegateHandle;

	FDelegateHandle DecideDelegateHandle;

	FDelegateHandle ActDelegateHandle;

	void Step();

	void Reset();

	void SendObserveRequest();

	void SendDecideRequest();

	void SendActRequest();
};
