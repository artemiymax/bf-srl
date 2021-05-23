#pragma once

#include "CoreMinimal.h"
#include "ISensor.h"

class UNREALAGENTSCORE_API FVectorSensor : public ISensor
{
public:
	explicit FVectorSensor(uint32 ObservationSize);

	void AddObservation(float Observation);

	void AddObservation(FVector2D Observation);

	void AddObservation(FVector Observation);

	void AddObservation(FVector4 Observation);

	void AddObservation(TArray<float> Observation);

	virtual void Update() override {}
	
	virtual void Read(TArray<float>& Reader) override;

	virtual void Reset() override;
private:
	TArray<float> Observations;
};

