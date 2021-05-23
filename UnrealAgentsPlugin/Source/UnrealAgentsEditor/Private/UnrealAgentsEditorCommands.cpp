#include "UnrealAgentsEditorCommands.h"

#define LOCTEXT_NAMESPACE "FUnrealAgentsEditorModule"

void FUnrealAgentsEditorCommands::RegisterCommands()
{
	UI_COMMAND(OpenEnvironmentSettingsWindow, "RL Environment Settings", "Show RL environment settings", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
