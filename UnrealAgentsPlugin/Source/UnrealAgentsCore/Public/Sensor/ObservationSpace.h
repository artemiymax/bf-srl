#pragma once

#include "CoreMinimal.h"
#include "Agent/AgentComponent.h"
#include "Algo/Accumulate.h"

class UNREALAGENTSCORE_API FObservationSpace
{	
public:
	FObservationSpace(const TArray<uint32> Shape, const TEnumAsByte<ESpaceType::Type> Type): Type(Type), Shape(Shape), Size(Algo::Accumulate(Shape, 0l))
	{
	}
	
	TEnumAsByte<ESpaceType::Type> Type;

	TArray<uint32> Shape;

	uint64 Size;
};
