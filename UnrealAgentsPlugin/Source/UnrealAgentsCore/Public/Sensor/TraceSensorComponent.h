#pragma once

#include "CoreMinimal.h"
#include "ISensorComponent.h"
#include "TraceSensor.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"

#include "TraceSensorComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNREALAGENTSCORE_API UTraceSensorComponent : public UActorComponent, public ISensorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(Category = TraceSensor, EditAnywhere)
	float TraceLength = 1000.f;
	
	UPROPERTY(Category = TraceSensor, EditAnywhere)
	float TraceRadius = 20.f;

	UPROPERTY(Category = TraceSensor, EditAnywhere)
	TEnumAsByte<ECollisionChannel> TraceChannel;

	UPROPERTY(Category = TraceSensor, EditAnywhere)
	TEnumAsByte<EDrawDebugTrace::Type> TraceDrawType;

	UPROPERTY(Category = TraceSensor, EditAnywhere)
	float PerceptionDegree = 7;

	UPROPERTY(Category = TraceSensor, EditAnywhere)
	float PerceptionRadius = 126;

	UPROPERTY(Category = TraceSensor, EditAnywhere)
	TArray<FName> PerceptionTags;

	UPROPERTY(Category = TraceSensor, EditAnywhere)
	uint32 StepBuffer;

	virtual FTraceSensor* GetSensor() override
	{
		return Sensor;
	}

	virtual void InitializeSensor() override;

	virtual void Reset() override;

private:
	FTraceSensor* Sensor;
};
