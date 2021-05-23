#pragma once

#include "CoreMinimal.h"
#include "IMetricsLogger.h"

enum UNREALAGENTSCORE_API EMetricAccumulationType
{
	None,
	TakeLast,
	Sum,
	Mean
};

class UNREALAGENTSCORE_API IMetricsWriter
{
public:
	virtual void AddMetric(const FString& Name, const float Value, const EMetricAccumulationType AccumulationType = None) = 0;

	virtual void AddMetric(const FString& Name, const FString& Value) = 0;

	virtual void RegisterLogger(const FString& Name, IMetricsLogger* MetricsLogger) = 0;

	virtual void Flush() = 0;

	virtual ~IMetricsWriter() = default;
};