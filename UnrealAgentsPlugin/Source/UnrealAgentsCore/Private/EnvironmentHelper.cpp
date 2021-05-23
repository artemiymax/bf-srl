#include "EnvironmentHelper.h"
#include "EnvironmentSettings.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Engine/World.h"
#include "Engine/GameEngine.h"

#if WITH_EDITOR
#include "UnrealEd.h"
#include "Editor/EditorEngine.h"
#endif

UWorld* FEnvironmentHelper::GetWorld()
{
	UWorld* World;
	
#if WITH_EDITOR
	UEditorEngine* EditorEngine = Cast<UEditorEngine>(GEngine);
	if (EditorEngine != nullptr)
	{
		World = EditorEngine->PlayWorld;
		if (World != nullptr && World->IsValidLowLevel() && World->IsGameWorld())
		{
			return World;
		}
	}
#endif
	
	UGameEngine* GameEngine = Cast<UGameEngine>(GEngine);
	if (GameEngine != nullptr)
	{
		World = GameEngine->GetGameWorld();
		if (World != nullptr && World->IsValidLowLevel())
		{
			return World;
		}
	}

	return nullptr;
}

UEnvironmentSettings* FEnvironmentHelper::GetSettings()
{
	const FString PackageName = FString("/Game/UnrealAgents");
	const FString PackageFileName = FPackageName::LongPackageNameToFilename(PackageName, FPackageName::GetAssetPackageExtension());
	const FString SettingsName = FString("EnvironmentSettings");
	
	UPackage* PluginPackage = LoadPackage(nullptr, *PackageName, LOAD_None);
	if(!PluginPackage)
	{
		PluginPackage = CreatePackage(*PackageName);
	}
	
	UEnvironmentSettings* Settings = LoadObject<UEnvironmentSettings>(PluginPackage, *SettingsName);
	if(!Settings)
	{
		Settings = NewObject<UEnvironmentSettings>(PluginPackage, *SettingsName, RF_Public | RF_Standalone);

		FAssetRegistryModule::AssetCreated(Settings);
		PluginPackage->MarkPackageDirty();

		bool bSaved = UPackage::SavePackage(PluginPackage, Settings, RF_Public | RF_Standalone, *PackageFileName, GError, nullptr, true, true, SAVE_NoError);
	}
		
	return Settings;
}

template <class ActorType>
ActorType* FEnvironmentHelper::GetActorByName(const FString& Name)
{
	UWorld* World = GetWorld();
	ActorType* targetActor = nullptr;
	TActorIterator<ActorType> ActorItr = TActorIterator<ActorType>(World);
	
	while (ActorItr)
	{
		if (ActorItr->GetName() == Name) 
		{
			targetActor = *ActorItr;
			break;
		}

		++ActorItr;
	}

	return targetActor;
}

template <class ComponentType>
TArray<ComponentType*> FEnvironmentHelper::GetComponentsByClass()
{
	UWorld* World = GetWorld();
	TArray<ComponentType*> TargetComponents;
	TActorIterator<AActor> ActorItr = TActorIterator<AActor>(World);
	
	while (ActorItr)
	{
		UActorComponent* TargetComponent = ActorItr->GetComponentByClass(ComponentType::StaticClass());
		if (TargetComponent) 
		{
			TargetComponents.Add(Cast<ComponentType>(TargetComponent));
		}

		++ActorItr;
	}

	return TargetComponents;
}
