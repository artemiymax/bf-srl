#pragma once

#include "CoreMinimal.h"
#include "Agent/AgentComponent.h"
#include "Engine/StaticMeshActor.h"
#include "PhysicsEngine/PhysicsConstraintActor.h"
#include "Sensor/VectorSensorComponent.h"
#include "CartPoleAgent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PLUGINTESTPROJECT_API UCartPoleAgent : public UAgentComponent
{
	GENERATED_BODY()

public:
	UCartPoleAgent();

	UPROPERTY(Category = Agent, EditInstanceOnly)
	APhysicsConstraintActor* SwingSensor;

	UPROPERTY(Category = Agent, VisibleAnywhere)
	UVectorSensorComponent* SensorComponent;

	UPROPERTY(Category = Agent, EditInstanceOnly)
	float PushForce = 300.f;

	virtual void OnBeginEpisode() override;

	virtual void OnObserve() override;

	virtual void OnAct(const FAgentAction& Action) override;

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

private:
	UPROPERTY(Category = Agent, VisibleInstanceOnly)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(Category = Agent, EditInstanceOnly)
	AStaticMeshActor* Pole;

	FVector DefaultCartLoc;

	FVector DefaultPoleLoc;
};

