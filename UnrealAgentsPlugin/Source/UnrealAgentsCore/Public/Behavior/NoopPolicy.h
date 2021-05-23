#pragma once

#include "IGlobalPolicy.h"
#include "IPolicy.h"

class FNoopGlobalPolicy final : public IGlobalPolicy
{
public:
	virtual void Initialize() override {}
	virtual void UpdateState(const FGEAgentState& AgentState) override {}
	virtual FAgentAction DecideAction() override { return FAgentAction(); }
};

class FNoopPolicy final : public IPolicy
{
public:
	virtual void Initialize(const FActionSpace& ActionSpace) override {}
	virtual void UpdateState(const FAgentState& AgentState) override {}
	virtual FAgentAction DecideAction() override { return FAgentAction(); }
};
