#pragma once

#include "CoreMinimal.h"
#include "Metrics/MetricsLoggerType.h"
#include "EnvironmentSettings.generated.h"

UCLASS()
class UNREALAGENTSCORE_API UEnvironmentSettings : public UObject
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(Category = Stepping, EditAnywhere)
	bool bUseAutoStep = true;

	UPROPERTY(Category = Stepping, EditAnywhere, meta=(EditCondition="bUseAutoStep"))
	uint32 StepFrequency = 0.f;

	UPROPERTY(Category = Stepping, EditAnywhere)
	bool bUseFixedTimeStep = true;

	UPROPERTY(Category = Stepping, EditAnywhere, meta=(EditCondition="bUseFixedTimeStep"))
	float FixedTimeStep = 0.f;

	UPROPERTY(Category = Metrics, EditAnywhere)
	bool bRecordMetrics = true;

	UPROPERTY(Category = Metrics, EditAnywhere, meta=(EditCondition="bRecordMetrics", UIMin = "0"))
	float FlushInterval = 5.f;

	UPROPERTY(Category = Metrics, EditAnywhere, meta=(EditCondition="bRecordMetrics"))
	TEnumAsByte<EMetricsLoggerType::Type> DefaultLogger = EMetricsLoggerType::Tensorboard;

	UPROPERTY(Category = RemoteChannel, EditAnywhere)
	bool bUseRemoteControl = true;
	
	UPROPERTY(Category = RemoteChannel, EditAnywhere, meta=(EditCondition="bUseRemoteControl", UIMin = "0", UIMax = "65535"))
	uint16 ProxyPort = 0;
};