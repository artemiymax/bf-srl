#include "EnvironmentTicker.h"

#include "EnvironmentDirector.h"
#include "EnvironmentHelper.h"
#include "Kismet/GameplayStatics.h"

UEnvironmentTicker::UEnvironmentTicker()
{
}

void UEnvironmentTicker::Tick(const float DeltaTime)
{
	StepDeltaTime += DeltaTime;
	StepTicks++;

	if(StepTicks >= FEnvironmentDirector::Get().GetSettings()->StepFrequency)
	{
		FEnvironmentDirector::Get().Step();

		StepDeltaTime = 0.f;
		StepTicks = 0;
	}
}

bool UEnvironmentTicker::IsTickable() const
{
	UWorld* World = FEnvironmentHelper::GetWorld();
	
	return IsValid(World) &&
		World->HasBegunPlay() &&
		FEnvironmentDirector::Get().IsInitialized() &&
		FEnvironmentDirector::Get().GetSettings()->bUseAutoStep;
}

bool UEnvironmentTicker::IsTickableInEditor() const
{
	return false;
}

TStatId UEnvironmentTicker::GetStatId() const
{
	return GetStatID();
}
