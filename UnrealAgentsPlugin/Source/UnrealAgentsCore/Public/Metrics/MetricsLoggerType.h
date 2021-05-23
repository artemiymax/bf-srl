#pragma once

#include "CoreMinimal.h"
#include "MetricsLoggerType.generated.h"

UENUM(BlueprintType)
namespace EMetricsLoggerType
{
	enum Type
	{
		None            UMETA(DisplayName = "None"),
		Tensorboard     UMETA(DisplayName = "Tensorboard")
	};
}