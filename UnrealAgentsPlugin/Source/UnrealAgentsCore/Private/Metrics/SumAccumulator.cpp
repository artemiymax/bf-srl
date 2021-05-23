#include "Metrics/SumAccumulator.h"

void FSumAccumulator::Update(const float Value)
{
	Sum += Value;
}

void FSumAccumulator::Update(const TArray<float>& Values)
{
	for(const float& Value : Values)
	{
		Sum += Value;
	};
}

void FSumAccumulator::Reset()
{
	Sum = 0.f;
}

float FSumAccumulator::GetResult()
{
	return Sum;
}
