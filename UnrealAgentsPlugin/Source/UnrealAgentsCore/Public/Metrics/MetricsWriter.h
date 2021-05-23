#pragma once

#include "CoreMinimal.h"
#include "IMetricsLogger.h"
#include "IMetricsWriter.h"

class FMetricsWriter : public IMetricsWriter
{
public:
	explicit FMetricsWriter(const float FlushInterval);

	virtual void AddMetric(const FString& Name, const float Value, const EMetricAccumulationType AccumulationType = None) override;

	virtual void AddMetric(const FString& Name, const FString& Value) override;

	virtual void RegisterLogger(const FString& Name, IMetricsLogger* MetricsLogger) override;

	virtual void Flush() override;

	virtual ~FMetricsWriter() override;

private:
	float FlushRate;
	
	FTimerHandle FlushTimerHandle;

	FDelegateHandle PostWorldInitializationHandle;
	
	TMap<FString, IMetricsLogger*> Loggers;

	TMap<FString, TArray<TTuple<float, uint64>>> ScalarMetrics;

	TMap<FString, TArray<TTuple<FString, uint64>>> TextMetrics;

	TMap<FString, EMetricAccumulationType> AccumulationMap;

	TTuple<float, uint64> AccumulateMetric(const TArray<TTuple<float, uint64>>& Values, const EMetricAccumulationType& AccumulationType);

	void FlushMetric(const FString& Name, const TArray<TTuple<float, uint64>>& Value);

	void FlushMetric(const FString& Name, const TArray<TTuple<FString, uint64>>& Value);
	
	void OnPostWorldInitialization(UWorld* World, UWorld::InitializationValues InitializationValues);
};

class FNoopMetricsWriter final : public IMetricsWriter
{
public:	
	FNoopMetricsWriter() {}

	virtual void AddMetric(const FString& Name, const float Value, const EMetricAccumulationType AccumulationType = None) override {}

	virtual void AddMetric(const FString& Name, const FString& Value) override {}

	virtual void RegisterLogger(const FString& Name, IMetricsLogger* MetricsLogger) override {}

	virtual void Flush() override {}
};
