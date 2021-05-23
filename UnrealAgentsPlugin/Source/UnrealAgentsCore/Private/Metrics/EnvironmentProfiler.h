#pragma once

#include "CoreMinimal.h"
#include "EngineUtils.h"
#include "EnvironmentDirector.h"
#include "EnvironmentHelper.h"
#include "Agent/AgentComponent.h"
#include "Metrics/MetricsWriter.h"

class FEnvironmentProfiler
{
public:
	static void UpdateMetrics()
	{
		static FDateTime PreviousUpdateTime;
		FDateTime CurrentUpdateTime = FDateTime::Now();

		IMetricsWriter* MetricsWriter = FEnvironmentDirector::Get().GetMetricsWriter();

		FCPUTime CpuTime = FPlatformTime::GetCPUTime();
		MetricsWriter->AddMetric(FString("simulation/cpu"), CpuTime.CPUTimePct, Mean);

		FPlatformMemoryStats MemoryStats = FPlatformMemory::GetStats();
		MetricsWriter->AddMetric(FString("simulation/memory"), MemoryStats.UsedPhysical / 1024.f, TakeLast);

		MetricsWriter->AddMetric(FString("simulation/delta"), 1.f / FEnvironmentHelper::GetWorld()->GetDeltaSeconds(), Mean);

		if (PreviousUpdateTime.GetTicks() != 0)
		{
			double Diff = (CurrentUpdateTime - PreviousUpdateTime).GetTotalMicroseconds();
			//GLog->Log(TEXT("---FPS---"));
			//GLog->Log(FString(std::to_string(Diff).c_str()));
			//GLog->Log(FString(std::to_string(Diff / 1000000.0).c_str()));
			//GLog->Log(FString(std::to_string(1/ (Diff / 1000000.0)).c_str()));

			float d1 = FEnvironmentHelper::GetWorld()->GetDeltaSeconds();

			TArray<float> Times;
			if (GEngine)
			{
				GEngine->GetAverageUnitTimes(Times);
			}
			
			MetricsWriter->AddMetric(FString("simulation/fps"), 1.f / (Diff / 1000000.f), Mean);
		}
		PreviousUpdateTime = CurrentUpdateTime;

		TArray<UAgentComponent*> EnvironmentAgents = FEnvironmentHelper::GetComponentsByClass<UAgentComponent>();
		MetricsWriter->AddMetric(FString("environment/agent_count"), EnvironmentAgents.Num(), TakeLast);
	}
};
