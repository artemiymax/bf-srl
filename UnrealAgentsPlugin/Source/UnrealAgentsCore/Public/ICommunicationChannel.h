#pragma once

#include"CoreMinimal.h"
#include "Agent/AgentAction.h"
#include "Agent/AgentState.h"

class ICommunicationChannel
{
public:
	virtual void Start(uint16 Port) = 0;
	
	virtual void Stop() = 0;

	virtual void RequestAction(const FAgentState& State) = 0;

	virtual FAgentAction WaitForAction(const FString& AgentId) = 0;

	virtual ~ICommunicationChannel() = default;
};
