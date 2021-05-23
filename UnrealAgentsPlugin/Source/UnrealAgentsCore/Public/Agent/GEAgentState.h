#pragma once

#include "CoreMinimal.h"

#include "AgentAction.h"
#include "AgentState.h"

struct UNREALAGENTSCORE_API FGEAgentState
{	
	TMap<FName, FAgentState> AgentStates;

	TMap<FName, FAgentAction> ActionMap;
};
