#include "Behavior/RemotePolicy.h"
#include "EnvironmentDirector.h"
#include "Sensor/ISensor.h"

URemotePolicy::URemotePolicy()
{
}

void URemotePolicy::Initialize(const FActionSpace& ActionSpace)
{
	RemoteChannel = FEnvironmentDirector::Get().GetRemoteChannel();
}

void URemotePolicy::UpdateState(const FAgentState& AgentState)
{
	AgentId = AgentState.AgentId;
	RemoteChannel->RequestAction(AgentState);
}

FAgentAction URemotePolicy::DecideAction()
{
	FAgentAction Action = RemoteChannel->WaitForAction(AgentId);
	return Action;
}
