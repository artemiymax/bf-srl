#pragma once

#include "CoreMinimal.h"

class UNREALAGENTSCORE_API FMovingAverageAccumulator
{
public:
	explicit FMovingAverageAccumulator(const uint32 Period);

	void Update(const float Value);

	void Reset();
	
	float GetResult();

private:
	TArray<float> Window;

	uint32 WindowIndex;
	
	uint32 WindowPeriod;

	float Sum;
};
