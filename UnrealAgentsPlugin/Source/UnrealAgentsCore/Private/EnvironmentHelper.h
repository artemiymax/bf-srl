#pragma once

#include "CoreMinimal.h"
#include "EnvironmentSettings.h"

class UNREALAGENTSCORE_API FEnvironmentHelper
{
public:
	static UWorld* GetWorld();

	static UEnvironmentSettings* GetSettings();
	
	template <class ActorType>
	static ActorType* GetActorByName(const FString& Name);
	
	template<class ComponentType>
	static TArray<ComponentType*> GetComponentsByClass();
};
