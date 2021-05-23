#pragma once

#include "CoreMinimal.h"
#include "ICommunicationChannel.h"
#include "IPolicy.h"
#include "Agent/ActionSpace.h"
#include "Agent/AgentAction.h"
#include "RemotePolicy.generated.h"

UCLASS(EditInlineNew, DefaultToInstanced)
class UNREALAGENTSCORE_API URemotePolicy : public UPolicyBase, public IPolicy
{
	GENERATED_BODY()

public:
	URemotePolicy();

	virtual void Initialize(const FActionSpace& ActionSpace) override;

	virtual void UpdateState(const FAgentState& AgentState) override;

	virtual FAgentAction DecideAction() override;

private:
	ICommunicationChannel* RemoteChannel;

	FString AgentId;
};
