#pragma once

#include "CoreMinimal.h"

#include "EnvironmentDirector.h"
#include "EnvironmentSettings.h"
#include "UnrealAgentsSubsystem.generated.h"

UCLASS()
class UUnrealAgentsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UUnrealAgentsSubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

	FEnvironmentDirector& GetEnvironmentDirector() const { return *EnvironmentDirector;  }

private:
	FEnvironmentDirector* EnvironmentDirector;
	
	UPROPERTY()
	UEnvironmentSettings* EnvironmentSettings;

	int32 ProxyPort;

	FDelegateHandle PostWorldInitializationHandle;

	void ProcessCommandLineArgs();
	
	void OnPostWorldInitialization(UWorld* World, UWorld::InitializationValues InitializationValues);
};