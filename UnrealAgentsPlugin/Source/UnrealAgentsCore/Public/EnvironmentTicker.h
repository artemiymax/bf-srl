#pragma once

#include "CoreMinimal.h"
#include "EnvironmentTicker.generated.h"

UCLASS()
class UEnvironmentTicker: public UObject, public FTickableGameObject
{
	GENERATED_BODY()
	
public:
	UEnvironmentTicker();
		
	virtual void Tick(float DeltaTime) override;
	
	virtual bool IsTickable() const override;
	
	virtual bool IsTickableInEditor() const override;
	
	virtual TStatId GetStatId() const override;

private:
	float StepDeltaTime;

	uint32 StepTicks;
};
