#pragma once

#include "CoreMinimal.h"
#include "SpaceType.generated.h"

UENUM(BlueprintType)
namespace ESpaceType
{
	enum Type
	{
		Discrete       UMETA(DisplayName = "Discrete"),
		Continuous     UMETA(DisplayName = "Continuous")
	};
}