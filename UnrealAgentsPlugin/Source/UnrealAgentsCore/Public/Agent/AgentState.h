#pragma once

#include "CoreMinimal.h"

class ISensor;

struct UNREALAGENTSCORE_API FAgentState
{
	FString AgentId;
	
	float StepReward = 0.f;

	float EpisodeReward = 0.f;
	
	bool bIsDone = false;

	TArray<ISensor*>* Sensors = nullptr;
};

