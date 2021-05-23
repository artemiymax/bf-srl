#pragma once

#include "ISensorComponent.generated.h"

UINTERFACE(MinimalAPI)
class USensorComponent : public UInterface
{
	GENERATED_BODY()
};

class ISensorComponent
{
	GENERATED_BODY()

public:
	virtual void InitializeSensor() = 0;

	virtual ISensor* GetSensor() = 0;

	virtual void Reset() = 0;
};

