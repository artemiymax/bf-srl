#include "Metrics/MetricsWriter.h"
#include "EnvironmentDirector.h"
#include "EnvironmentHelper.h"
#include "TensorboardMetricsLogger.h"
#include "Algo/Accumulate.h"
#include "Metrics/MeanAccumulator.h"
#include "Metrics/SumAccumulator.h"

FMetricsWriter::FMetricsWriter(const float FlushInterval)
{
	FlushRate = FlushInterval;

	UWorld* World = FEnvironmentHelper::GetWorld();
	if (IsValid(World))
	{
		OnPostWorldInitialization(World, UWorld::InitializationValues());
	}
	else
	{
		PostWorldInitializationHandle = FWorldDelegates::OnPostWorldInitialization.AddRaw(this, &FMetricsWriter::OnPostWorldInitialization);
	}
}

void FMetricsWriter::AddMetric(const FString& Name, const float Value, const EMetricAccumulationType AccumulationType)
{
	AccumulationMap.Add(Name, AccumulationType);

	const TTuple<float, uint64> Metric = TTuple<float, uint64>(Value, FEnvironmentDirector::Get().GetTotalStepCount());

	TArray<TTuple<float, uint64>>& MetricValues = ScalarMetrics.FindOrAdd(Name);
	MetricValues.Add(Metric);
}

void FMetricsWriter::AddMetric(const FString& Name, const FString& Value)
{
	const TTuple<FString, uint64> Metric = TTuple<FString, uint64>(Value, FEnvironmentDirector::Get().GetTotalStepCount());

	TArray<TTuple<FString, uint64>>& MetricValues = TextMetrics.FindOrAdd(Name);
	MetricValues.Add(Metric);
}

void FMetricsWriter::RegisterLogger(const FString& Name, IMetricsLogger* MetricsLogger)
{
	if (Loggers.Contains(Name))
	{
		UE_LOG(LogTemp, Warning, TEXT("Metric logger with name %s already registered"), *Name);
		return;
	}

	Loggers.Add(Name, MetricsLogger);
}

void FMetricsWriter::Flush()
{
	for (auto& Metric : ScalarMetrics)
	{
		FlushMetric(Metric.Key, Metric.Value);
	}

	for (auto& Metric : TextMetrics)
	{
		FlushMetric(Metric.Key, Metric.Value);
	}

	ScalarMetrics.Reset();
	TextMetrics.Reset();
}

FMetricsWriter::~FMetricsWriter()
{
	FWorldDelegates::OnPostWorldInitialization.Remove(PostWorldInitializationHandle);
	FEnvironmentHelper::GetWorld()->GetTimerManager().ClearTimer(FlushTimerHandle);
}

TTuple<float, uint64> FMetricsWriter::AccumulateMetric(const TArray<TTuple<float, uint64>>& Values, const EMetricAccumulationType& AccumulationType)
{
	const TTuple<float, uint64>& LastMetric = Values.Last();

	switch (AccumulationType)
	{
	case TakeLast:
		return Values.Last();
	case Sum:
	{
		FSumAccumulator SumAccumulator;
		for (auto& Value : Values)
		{
			SumAccumulator.Update(Value.Key);
		}

		return TTuple<float, uint64>(SumAccumulator.GetResult(), LastMetric.Value);
	}
	case Mean:
	{
		FMeanAccumulator MeanAccumulator;
		for (auto& Value : Values)
		{
			MeanAccumulator.Update(Value.Key);
		}

		return TTuple<float, uint64>(MeanAccumulator.GetResult(), LastMetric.Value);
	}
	default:
		return Values.Last();
	}
}

void FMetricsWriter::FlushMetric(const FString& Name, const TArray<TTuple<float, uint64>>& Value)
{
	EMetricAccumulationType& AccumulationType = AccumulationMap.FindOrAdd(Name);

	if (AccumulationType == None)
	{
		for (auto& Logger : Loggers)
		{
			Logger.Value->LogScalar(Name, Value);
		}
	}
	else
	{
		const TTuple<float, uint64> AccumulatedValue = AccumulateMetric(Value, AccumulationType);
		for (auto& Logger : Loggers)
		{
			Logger.Value->LogScalar(Name, AccumulatedValue);
		}
	}
}

void FMetricsWriter::FlushMetric(const FString& Name, const TArray<TTuple<FString, uint64>>& Value)
{
	for (auto& Logger : Loggers)
	{
		Logger.Value->LogString(Name, Value);
	}
}

void FMetricsWriter::OnPostWorldInitialization(UWorld* World, const UWorld::InitializationValues InitializationValues)
{
	World->GetTimerManager().SetTimer
	(
		FlushTimerHandle,
		FTimerDelegate::CreateRaw(this, &FMetricsWriter::Flush),
		FMath::Max(0.f, FlushRate),
		true
	);
}
