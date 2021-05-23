#pragma once

#include "CoreMinimal.h"
#include "Agent/AgentComponent.h"
#include "Engine/BlockingVolume.h"
#include "Engine/StaticMeshActor.h"
#include "Sensor/TraceSensorComponent.h"
#include "BallPusherAgent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PLUGINTESTPROJECT_API UBallPusherAgentComponent : public UAgentComponent
{
	GENERATED_BODY()

public:
	UBallPusherAgentComponent();

	UPROPERTY(Category = Agent, VisibleAnywhere)
	UTraceSensorComponent* SensorComponent;

	UPROPERTY(Category = Agent, EditAnywhere)
	ABlockingVolume* Goal;

	UPROPERTY(Category = Agent, EditAnywhere)
	AStaticMeshActor* Ball;

	UPROPERTY(Category = Agent, EditAnywhere)
	AStaticMeshActor* Arena;

	UPROPERTY(Category = Agent, EditAnywhere)
	AStaticMeshActor* LeftWall;

	UPROPERTY(Category = Agent, EditAnywhere)
	AStaticMeshActor* RightWall;

	UPROPERTY(Category = Agent, EditAnywhere)
	float MoveScale = 1.0f;

	UPROPERTY(Category = Agent, EditAnywhere)
	float RotateScale = 1.0f;

	UPROPERTY(Category = Agent, EditAnywhere)
	bool bShowTraces;

	virtual void OnBeginEpisode() override;

	virtual void OnObserve() override;

	virtual void OnAct(const FAgentAction& Action) override;

	UFUNCTION()
	void OnGoalHit(AActor* OverlappedActor, AActor* OtherActor);

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

private:
	FVector DefaultLoc;
	FRotator DefaultRot;

	FVector DefaultBallLoc;
	FRotator DefaultBallRot;

	FVector DefaultGoalLoc;
	FRotator DefaultGoalRot;
};

