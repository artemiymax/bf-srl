#pragma once

#include "CoreMinimal.h"
#include "ObservationSpace.h"

class ISensor
{
public:
	explicit ISensor(const FObservationSpace& ObservationSpace) : ObservationSpace(ObservationSpace)
	{
	}
	
	virtual void Update() = 0;
	
	virtual void Read(TArray<float>& Reader) = 0;

	virtual void Reset() = 0;

	virtual ~ISensor() = default;

protected:
	FObservationSpace ObservationSpace;
};

