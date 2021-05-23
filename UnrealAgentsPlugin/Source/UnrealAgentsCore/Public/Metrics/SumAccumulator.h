#pragma once

#include "CoreMinimal.h"

class UNREALAGENTSCORE_API FSumAccumulator
{
public:
	void Update(const float Value);

	void Update(const TArray<float>& Values);

	void Reset();
	
	float GetResult();

private:
	float Sum = 0.f;
};