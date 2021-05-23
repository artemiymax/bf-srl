#include "Metrics/MeanAccumulator.h"

void FMeanAccumulator::Update(const float Value)
{
	Sum += Value;
	Count += 1;
}

void FMeanAccumulator::Update(const TArray<float>& Values)
{
	for(const float& Value : Values)
	{
		Sum += Value;
	};

	Count += Values.Num();
}

void FMeanAccumulator::Reset()
{
	Sum = 0.f;
	Count = 0;
}

float FMeanAccumulator::GetResult()
{
	if (Count == 0)
	{
		return 0.f;
	}

	return Sum / Count;
}
