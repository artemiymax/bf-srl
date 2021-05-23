#pragma once

#include "CoreMinimal.h"

#include "EnvironmentDirector.h"
#include "Metrics/IMetricsWriter.h"
#include "Behavior/IGlobalPolicy.h"
#include "Agent/AgentAction.h"
#include "Agent/GEAgentState.h"
#include "Metrics/MovingAverageAccumulator.h"

#include "BallPusherCurriculumPolicy.generated.h"

UCLASS(EditInlineNew, DefaultToInstanced)
class UBallPusherCurriculumPolicy : public UGlobalPolicyBase, public IGlobalPolicy
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TArray<float> LevelThresholds;

	virtual void Initialize() override
	{
	}

	virtual void UpdateState(const FGEAgentState& AgentState) override
	{
		CurrentCurriculum.Reset();

		for (const TPair<FName, FAgentState>& Agent : AgentState.AgentStates)
		{
			if (Agent.Value.bIsDone)
			{
				UpdateCurriculum(Agent.Key, Agent.Value);
			}
		}
	}

	virtual FAgentAction DecideAction() override
	{
		return FAgentAction(CurrentCurriculum, {});
	}

	void UpdateCurriculum(const FName& AgentId, const FAgentState& AgentState)
	{
		TTuple<uint32, FMovingAverageAccumulator, uint32>* LevelProgress = ProgressMap.Find(AgentId);

		if (LevelProgress == nullptr)
		{
			TTuple<uint32, FMovingAverageAccumulator, uint32> Progress(1, FMovingAverageAccumulator(50), 1);
			ProgressMap.Add(AgentId, Progress);

			LevelProgress = ProgressMap.Find(AgentId);
		}

		bool bIsLevelChanged = false;
		for (uint32 i = LevelThresholds.Num(); i > 1; i--)
		{
			if (IsReadyForLevel(LevelProgress, i))
			{
				MoveToNextLevel(LevelProgress);
				bIsLevelChanged = true;
				break;
			}
		}

		if (!bIsLevelChanged)
		{
			LevelProgress->Get<1>().Update(AgentState.EpisodeReward);
			LevelProgress->Get<2>() += 1;
			CurrentCurriculum.Add(LevelProgress->Get<0>());
		}

		FString CurriculumMetric = FString::Format(TEXT("agent/{0}/curriculum"), { AgentId.ToString() });
		IMetricsWriter* MetricsWriter = FEnvironmentDirector::Get().GetMetricsWriter();
		MetricsWriter->AddMetric(CurriculumMetric, LevelProgress->Get<0>(), TakeLast);
	}

private:
	TArray<uint32> CurrentCurriculum;

	TMap<FName, TTuple<uint32, FMovingAverageAccumulator, uint32>> ProgressMap;

	bool IsReadyForLevel(TTuple<uint32, FMovingAverageAccumulator, uint32>* Progress, uint32 Level)
	{
		return Progress->Get<0>() == Level - 1 && Progress->Get<1>().GetResult() > LevelThresholds[Level - 1] && Progress->Get<2>() >= 50;
	}

	void MoveToNextLevel(TTuple<uint32, FMovingAverageAccumulator, uint32>* Progress)
	{
		Progress->Get<1>().Reset();
		Progress->Get<2>() = 0;
		Progress->Get<0>() = Progress->Get<0>() + 1;
		CurrentCurriculum.Add(Progress->Get<0>());
	}
};
