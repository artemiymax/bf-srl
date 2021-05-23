#pragma once

#include "CoreMinimal.h"
#include "BallPusherAgent.h"
#include "Engine/StaticMeshActor.h"
#include "Agent/GEAgentComponent.h"
#include "CurriculumAgent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PLUGINTESTPROJECT_API UCurriculumAgentComponent : public UGEAgentComponent
{
	GENERATED_BODY()

public:
	UCurriculumAgentComponent();

	virtual void OnObserve() override;

	virtual void OnAct(const FAgentAction& Action) override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

private:
	UPROPERTY()
	TArray<UBallPusherAgentComponent*> Agents;

	TArray<uint32> LevelMap;

	void ChangeLevel(int32 ActorIndex, uint32 Level);

	bool bIsDisabled;
};