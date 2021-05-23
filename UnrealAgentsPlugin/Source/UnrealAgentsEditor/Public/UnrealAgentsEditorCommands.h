#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "UnrealAgentsEditorStyle.h"

class FUnrealAgentsEditorCommands : public TCommands<FUnrealAgentsEditorCommands>
{
public:
	FUnrealAgentsEditorCommands()
		: TCommands<FUnrealAgentsEditorCommands>(TEXT("UnrealAgentsEditor"), NSLOCTEXT("Contexts", "UnrealAgentsEditor", "UnrealAgentsEditor Plugin"), NAME_None, FUnrealAgentsEditorStyle::GetStyleSetName())
	{
	}

	virtual void RegisterCommands() override;

	TSharedPtr< FUICommandInfo > OpenEnvironmentSettingsWindow;
};