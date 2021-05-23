#pragma once

#include "CoreMinimal.h"
#include "Agent/AgentAction.h"
#include "Agent/GEAgentState.h"

#include "IGlobalPolicy.generated.h"

UINTERFACE(MinimalAPI)
class UGlobalPolicy : public UInterface
{
    GENERATED_BODY()
};

class UNREALAGENTSCORE_API IGlobalPolicy
{
	GENERATED_BODY()
	
public:
	virtual void Initialize() = 0;
	
	virtual void UpdateState(const FGEAgentState& AgentState) = 0;
	
	virtual FAgentAction DecideAction() = 0;
};

UCLASS(Abstract, EditInlineNew, DefaultToInstanced)
class UNREALAGENTSCORE_API UGlobalPolicyBase : public UObject
{
	GENERATED_BODY()
};