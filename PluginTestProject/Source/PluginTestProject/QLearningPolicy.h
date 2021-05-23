#pragma once

#include "CoreMinimal.h"

#include "EnvironmentDirector.h"
#include "Behavior/IPolicy.h"
#include "Agent/ActionSpace.h"
#include "Agent/AgentAction.h"
#include "Agent/AgentState.h"
#include "Behavior/BehaviorComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Metrics/MetricsWriter.h"
#include "Sensor/ISensor.h"

#include "QLearningPolicy.generated.h"

UCLASS()
class UQLearningPolicy : public UPolicyBase, public IPolicy
{
	GENERATED_BODY()

public:
	FString StateString;
	TArray<float> Observations;
	uint32 LastAction;

	UPROPERTY(EditAnywhere)
	float Discount;

	UPROPERTY(EditAnywhere)
	float Alpha;
	
	UPROPERTY(EditAnywhere)
	float Epsilon;

	UPROPERTY(EditAnywhere)
	float EpsilonDecay;

	UPROPERTY(EditAnywhere)
	bool bUseGlobalTable;

	uint32 ActionSize;

	TMap<FString, TArray<float>> QTable;
	
	virtual void Initialize(const FActionSpace& ActionSpace) override
	{
		StateString = "";
		ActionSize = 0;
		LastAction = -1;
		GetQTable().Reset();
		
		for(uint32 ActionDimension : ActionSpace.Shape)
		{
			ActionSize += ActionDimension;
		}

		FString ParametersTable = "| Q Policy Parameter | Value |\n---|---\n";
		ParametersTable += FString::Format(TEXT("| Alpha | {0} |\n"), { Alpha });
		ParametersTable += FString::Format(TEXT("| Discount | {0} |\n"), { Discount });
		ParametersTable += FString::Format(TEXT("| Epsilon | {0} |\n"), { Epsilon });
		ParametersTable += FString::Format(TEXT("| Epsilon Decay | {0} |\n"), { EpsilonDecay });
		ParametersTable += FString::Format(TEXT("| GlobalTable | {0} |\n"), { bUseGlobalTable });

		FString AgentName = Cast<UBehaviorComponent>(GetOuter())->GetOwner()->GetName();
		
		FEnvironmentDirector::Get().GetMetricsWriter()->AddMetric(FString::Format(TEXT("Policy/{0}/QLearningPolicy/Parameters"), {AgentName}), ParametersTable);
	}

	virtual void UpdateState(const FAgentState& AgentState) override
	{
		Observations.Reset();
		AgentState.Sensors->Last()->Read(Observations);

		FString NextStateString = "";
		/*for(float Observation: Observations)
		{
			int32 ClampedObservation = FMath::FloorToInt(Observation / 0.1f);
			NextStateString += FString::FromInt(ClampedObservation);
		}*/

		//NextStateString += FString::FromInt(FMath::FloorToInt(Observations[0] / 1.f));
		//NextStateString += FString::FromInt(FMath::FloorToInt(Observations[1] / 1.f));
		
		//NextStateString += FString::FromInt(FMath::FloorToInt(Observations[2] / 0.1f));
		//NextStateString += FString::FromInt(FMath::FloorToInt(Observations[3] / 1.f));

		NextStateString += FString::FromInt(FMath::FloorToInt(Observations[0] / 1.f));
		NextStateString += FString::FromInt(FMath::FloorToInt(Observations[1] / 5.f));
		NextStateString += FString::FromInt(FMath::FloorToInt(Observations[2] / 0.01f));
		//NextStateString += FString::FromInt(FMath::FloorToInt(Observations[3] / 1.f));
		NextStateString += FString::FromInt(FMath::FloorToInt(Observations[3] / 0.1f));

		if (AgentState.bIsDone)
		{
			if(Epsilon - EpsilonDecay >= 0)
			{
				Epsilon -= EpsilonDecay;
			}

			UpdatePolicyMetrics();
		}
		
		UpdateQTable(StateString, NextStateString, LastAction, AgentState.StepReward);
		StateString = NextStateString;
	}

	virtual FAgentAction DecideAction() override
	{
		if (FMath::FRandRange(0.f, 1.f) <= Epsilon)
		{
			int32 RandomAction = FMath::RandRange(0, static_cast<int32>(ActionSize) - 1);
			LastAction = static_cast<uint32>(RandomAction);
		}
		else
		{
			int32 BestActionIndex;
			float BestActionValue;
			UKismetMathLibrary::MaxOfFloatArray(GetQTable()[StateString], BestActionIndex, BestActionValue);
			LastAction = static_cast<uint32>(BestActionIndex);
		}

		return FAgentAction({LastAction}, {});
	}

	void UpdateQTable(FString PreviousState, FString NextState, uint32 Action, float Reward)
	{
		if (LastAction == -1)
		{
			TrySetDefaultValues(NextState);
			return;
		}

		TrySetDefaultValues(NextState);
		
		int32 NextBestActionIndex;
		float NextBestActionValue;
		UKismetMathLibrary::MaxOfFloatArray(GetQTable()[NextState], NextBestActionIndex, NextBestActionValue);

		float Target = Reward + Discount * NextBestActionValue;
		float Delta = Target - GetQTable()[PreviousState][Action];
		GetQTable()[PreviousState][Action] += Alpha * Delta;
	}

	void TrySetDefaultValues(const FString State)
	{
		if (!GetQTable().Contains(State))
		{
			TArray<float> Probabilities;
			Probabilities.Init(0.f, ActionSize);
			
			for(uint32 i = 0; i < ActionSize; i++)
			{
				Probabilities[i] = FMath::FRandRange(0.f, 1.f);
			}
		
			GetQTable().Add(State, Probabilities);
		}
	}

	TMap<FString, TArray<float>>& GetQTable()
	{
		if (bUseGlobalTable)
		{
			static TMap<FString, TArray<float>> GlobalQTable{};
			return GlobalQTable;
		}

		return QTable;
	}

	void UpdatePolicyMetrics()
	{
		IMetricsWriter* MetricsWriter = FEnvironmentDirector::Get().GetMetricsWriter();
		MetricsWriter->AddMetric(FString("Policy/QLearningPolicy/State"), GetQTable().Num(), TakeLast);
		MetricsWriter->AddMetric(FString("Policy/QLearningPolicy/Epsilon"), Epsilon, TakeLast);
	}
};
