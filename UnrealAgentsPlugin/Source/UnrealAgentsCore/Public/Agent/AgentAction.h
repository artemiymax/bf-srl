#pragma once

#include "CoreMinimal.h"

class UNREALAGENTSCORE_API FAgentAction
{
public:	
	FAgentAction()
	{
	}

	FAgentAction(const TArray<uint32> DiscreteValues, const TArray<float> ContinuousValues) :
	DiscreteValues(DiscreteValues),
	ContinuousValues(ContinuousValues)
	{
	}
	
	TArray<uint32> DiscreteValues;

	TArray<float> ContinuousValues;
};