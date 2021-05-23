#include "UnrealAgentsSubsystem.h"
#include "EnvironmentDirector.h"
#include "EnvironmentHelper.h"
#include "Kismet/GameplayStatics.h"

UUnrealAgentsSubsystem::UUnrealAgentsSubsystem()
{
}

void UUnrealAgentsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	EnvironmentSettings = FEnvironmentHelper::GetSettings();
	EnvironmentSettings->AddToRoot();

	ProcessCommandLineArgs();

	EnvironmentDirector = new FEnvironmentDirector();
	EnvironmentDirector->Initialize(EnvironmentSettings);

	UWorld* World = FEnvironmentHelper::GetWorld();
	if (IsValid(World))
	{
		OnPostWorldInitialization(World, UWorld::InitializationValues());
	}
	else
	{
		PostWorldInitializationHandle = FWorldDelegates::OnPostWorldInitialization.AddUObject(this, &UUnrealAgentsSubsystem::OnPostWorldInitialization);
	}
}

void UUnrealAgentsSubsystem::Deinitialize()
{
	delete EnvironmentDirector;
	FWorldDelegates::OnPostWorldInitialization.Remove(PostWorldInitializationHandle);
}

void UUnrealAgentsSubsystem::ProcessCommandLineArgs()
{
	const FString Cmd = FCommandLine::Get();

	FString ProxyPortArg;
	if (FParse::Value(*Cmd, TEXT("proxyPort"), ProxyPortArg))
	{
		ProxyPortArg = ProxyPortArg.Replace(TEXT("="), TEXT(""));
	}

	if (!ProxyPortArg.IsEmpty())
	{
		EnvironmentSettings->ProxyPort = FCString::Atoi(*ProxyPortArg);
	}

	if (FParse::Param(*Cmd, TEXT("recordMetrics")))
	{
		EnvironmentSettings->bRecordMetrics = true;
	}

	FString FlushInterval;
	if (FParse::Value(*Cmd, TEXT("flushInterval"), FlushInterval))
	{
		FlushInterval = FlushInterval.Replace(TEXT("="), TEXT(""));
	}

	if (!FlushInterval.IsEmpty())
	{
		EnvironmentSettings->FlushInterval = FCString::Atoi(*FlushInterval);
	}
}

void UUnrealAgentsSubsystem::OnPostWorldInitialization(UWorld* World, const UWorld::InitializationValues InitializationValues)
{
	const FString Cmd = FCommandLine::Get();
	
	FString MapArg;
	if (FParse::Value(*Cmd, TEXT("map"), MapArg))
	{
		MapArg = MapArg.Replace(TEXT("="), TEXT(""));
	}

	FString MapName = World->GetMapName();
	GLog->Log("MAP_NAME");
	GLog->Log(MapName);
	
	if (!MapArg.IsEmpty() && World->GetMapName() != MapArg)
	{
		UGameplayStatics::OpenLevel(World, FName(MapArg));
	}
}
