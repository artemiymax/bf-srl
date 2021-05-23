#pragma once

#include "CoreMinimal.h"

class FUnrealAgentsCoreModule final : public IModuleInterface
{
public:
	virtual void StartupModule() override;

	virtual void ShutdownModule() override;

private:
	void AgentCommand(const TArray<FString>& Args);
};
