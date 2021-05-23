#pragma once

#include "CoreMinimal.h"
#include "EnvironmentTicker.h"
#include "EnvironmentSettings.h"
#include "ICommunicationChannel.h"
#include "Metrics/IMetricsWriter.h"

class UNREALAGENTSCORE_API FEnvironmentDirector
{
public:
	bool IsInitialized() const { return bIsInitialized; }
	
	uint32 GetEpisodeCount() const { return EpisodeCount; }

	uint32 GetEpisodeStepCount() const { return EpisodeStepCount; }

	uint32 GetTotalStepCount() const { return TotalStepCount; }

	UEnvironmentSettings* GetSettings() const { return EnvironmentSettings; }

	IMetricsWriter* GetMetricsWriter() const { return MetricsWriter; }

	ICommunicationChannel* GetRemoteChannel() const { return RemoteChannel; }

	static FEnvironmentDirector& Get();

	FEnvironmentDirector();

	void Initialize(UEnvironmentSettings* Settings);

	void Reset();
	
	void Step();

	~FEnvironmentDirector();

	DECLARE_EVENT(FEnvironmentDirector, FOnGlobalResetActionEvent)
	FOnGlobalResetActionEvent& OnGlobalResetAction() { return OnGlobalResetActionEvent; }

	DECLARE_EVENT(FEnvironmentDirector, FOnGlobalStepActionEvent)
	FOnGlobalStepActionEvent& OnGlobalStepAction() { return OnGlobalStepActionEvent; }
	
	DECLARE_EVENT(FEnvironmentDirector, FOnGlobalObserveActionEvent)
	FOnGlobalObserveActionEvent& OnGlobalObserveAction() { return OnGlobalObserveActionEvent; }

	DECLARE_EVENT(FEnvironmentDirector, FOnGlobalDecideActionEvent)
	FOnGlobalDecideActionEvent& OnGlobalDecideAction() { return OnGlobalDecideActionEvent; }

	DECLARE_EVENT(FEnvironmentDirector, FOnGlobalActActionEvent)
	FOnGlobalActActionEvent& OnGlobalActAction() { return OnGlobalActActionEvent; }

	
	DECLARE_EVENT(FEnvironmentDirector, FOnResetActionEvent)
	FOnResetActionEvent& OnResetAction() { return OnResetActionEvent; }

	DECLARE_EVENT(FEnvironmentDirector, FOnStepActionEvent)
	FOnStepActionEvent& OnStepAction() { return OnStepActionEvent; }
	
	DECLARE_EVENT(FEnvironmentDirector, FOnObserveActionEvent)
	FOnObserveActionEvent& OnObserveAction() { return OnObserveActionEvent; }

	DECLARE_EVENT(FEnvironmentDirector, FOnDecideActionEvent)
	FOnDecideActionEvent& OnDecideAction() { return OnDecideActionEvent; }

	DECLARE_EVENT(FEnvironmentDirector, FOnActActionEvent)
	FOnActActionEvent& OnActAction() { return OnActActionEvent; }

private:
	bool bIsInitialized;
	
	bool bIsFirstStep;
	
	uint32 EpisodeCount;
	
	uint32 EpisodeStepCount;

	uint32 TotalStepCount;

	UEnvironmentSettings* EnvironmentSettings;

	UEnvironmentTicker* Ticker;

	IMetricsWriter* MetricsWriter;

	ICommunicationChannel* RemoteChannel;

	class FOnGlobalResetActionEvent OnGlobalResetActionEvent;
	class FOnGlobalStepActionEvent OnGlobalStepActionEvent;
	class FOnGlobalObserveActionEvent OnGlobalObserveActionEvent;
	class FOnGlobalDecideActionEvent OnGlobalDecideActionEvent;
	class FOnGlobalActActionEvent OnGlobalActActionEvent;
	
	class FOnResetActionEvent OnResetActionEvent;
	class FOnStepActionEvent OnStepActionEvent;
	class FOnObserveActionEvent OnObserveActionEvent;
	class FOnDecideActionEvent OnDecideActionEvent;
	class FOnActActionEvent OnActActionEvent;

	void StepGlobalAgent();

	void StepWorldAgents();
	
	void UpdateMetrics();
};
