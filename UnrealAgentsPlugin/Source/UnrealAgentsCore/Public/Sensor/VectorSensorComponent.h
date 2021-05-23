#pragma once

#include "CoreMinimal.h"
#include "ISensorComponent.h"
#include "VectorSensor.h"
#include "Components/ActorComponent.h"

#include "VectorSensorComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNREALAGENTSCORE_API UVectorSensorComponent : public UActorComponent, public ISensorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(Category = VectorSensor, EditInstanceOnly)
	uint32 ObservationSize;

	virtual FVectorSensor* GetSensor() override
	{
		return Sensor;
	}

	virtual void InitializeSensor() override;

	virtual void Reset() override;

private:
	FVectorSensor* Sensor;
};
