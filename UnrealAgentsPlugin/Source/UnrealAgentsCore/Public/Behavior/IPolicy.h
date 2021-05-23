#pragma once

#include "CoreMinimal.h"
#include "Agent/ActionSpace.h"
#include "Agent/AgentAction.h"
#include "Agent/AgentState.h"

#include "IPolicy.generated.h"

UINTERFACE(MinimalAPI)
class UPolicy : public UInterface
{
    GENERATED_BODY()
};

class UNREALAGENTSCORE_API IPolicy
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(const FActionSpace& ActionSpace) = 0;
	
	virtual void UpdateState(const FAgentState& AgentState) = 0;
	
	virtual FAgentAction DecideAction() = 0;
};

UCLASS(Abstract, EditInlineNew, DefaultToInstanced)
class UNREALAGENTSCORE_API UPolicyBase : public UObject
{
	GENERATED_BODY()
};