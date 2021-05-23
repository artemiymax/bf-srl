#pragma once

#include "CoreMinimal.h"

class IMetricsLogger
{
public:
	virtual void LogScalar(const FString& Name, const TTuple<float, uint64>& Value) = 0;

	virtual void LogScalar(const FString& Name, const TArray<TTuple<float, uint64>>& Values) = 0;

	virtual void LogString(const FString& Name, const TTuple<FString, uint64>& Value) = 0;

	virtual void LogString(const FString& Name, const TArray<TTuple<FString, uint64>>& Values) = 0;

	virtual ~IMetricsLogger() = default;
};