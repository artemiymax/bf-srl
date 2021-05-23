#pragma once

#include "CoreMinimal.h"
#include "SpaceType.h"

class UNREALAGENTSCORE_API FActionSpace
{	
public:
	FActionSpace(const TEnumAsByte<ESpaceType::Type> ActionType, const TArray<uint32> ActionShape)
	{
		Type = ActionType;
		Shape = ActionShape;
	}
	
	TEnumAsByte<ESpaceType::Type> Type;

	TArray<uint32> Shape;
};