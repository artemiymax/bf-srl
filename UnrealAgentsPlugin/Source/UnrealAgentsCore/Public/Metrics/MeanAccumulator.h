#pragma once

#include "CoreMinimal.h"

class UNREALAGENTSCORE_API FMeanAccumulator
{
public:
	void Update(const float Value);

	void Update(const TArray<float>& Values);

	void Reset();

	float GetResult();

private:
	uint32 Count = 0;

	float Sum = 0;
};