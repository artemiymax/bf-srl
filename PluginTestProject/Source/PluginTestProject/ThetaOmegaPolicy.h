#pragma once

#include "CoreMinimal.h"
#include "Behavior/IPolicy.h"
#include "Agent/ActionSpace.h"
#include "Agent/AgentAction.h"
#include "Agent/AgentState.h"
#include "Sensor/ISensor.h"

#include "ThetaOmegaPolicy.generated.h"

UCLASS(EditInlineNew, DefaultToInstanced)
class UThetaOmegaPolicy : public UPolicyBase, public IPolicy
{
	GENERATED_BODY()

public:
	TArray<float> Observations;
	
	virtual void Initialize(const FActionSpace& ActionSpace) override
	{
		
	}

	virtual void UpdateState(const FAgentState& AgentState) override
	{
		Observations.Reset();
		AgentState.Sensors->Last()->Read(Observations);
	}

	virtual FAgentAction DecideAction() override
	{
		float Theta = Observations[2];
		float Omega = Observations[3];

		if (FMath::Abs(Theta) > 0.3f)
		{
			return Theta > 0.f ? FAgentAction({1}, {}) : FAgentAction({0}, {});
		}
		
		return Omega > 0.f ? FAgentAction({1}, {}) : FAgentAction({0}, {});
	}
};
