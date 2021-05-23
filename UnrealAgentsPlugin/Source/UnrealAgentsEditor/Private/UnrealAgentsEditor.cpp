#include "UnrealAgentsEditor.h"

#include "EnvironmentHelper.h"
#include "EnvironmentSettings.h"
#include "UnrealAgentsEditorStyle.h"
#include "UnrealAgentsEditorCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "ToolMenus.h"

static const FName EnvironmentSettingsTabName("EnvironmentSettings");

#define LOCTEXT_NAMESPACE "FUnrealAgentsEditorModule"

void FUnrealAgentsEditorModule::StartupModule()
{	
	FUnrealAgentsEditorStyle::Initialize();
	FUnrealAgentsEditorStyle::ReloadTextures();

	FUnrealAgentsEditorCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FUnrealAgentsEditorCommands::Get().OpenEnvironmentSettingsWindow,
		FExecuteAction::CreateRaw(this, &FUnrealAgentsEditorModule::EnvironmentSettingsButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FUnrealAgentsEditorModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(EnvironmentSettingsTabName, FOnSpawnTab::CreateRaw(this, &FUnrealAgentsEditorModule::OnSpawnEnvironmentSettingsTab))
		.SetDisplayName(LOCTEXT("FUnrealAgentsEditorEnvironmentSettingsTabTitle", "RL Environment Settings"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FUnrealAgentsEditorModule::ShutdownModule()
{
	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FUnrealAgentsEditorStyle::Shutdown();

	FUnrealAgentsEditorCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(EnvironmentSettingsTabName);
}

TSharedRef<SDockTab> FUnrealAgentsEditorModule::OnSpawnEnvironmentSettingsTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FPropertyEditorModule& PropPlugin = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs DetailsViewArgs(false, false, true, FDetailsViewArgs::HideNameArea, false);
	DetailsViewArgs.bShowActorLabel = false;

	EnvironmentSettingsView = PropPlugin.CreateDetailView(DetailsViewArgs);

	UEnvironmentSettings* Settings = FEnvironmentHelper::GetSettings();
	if (Settings)
	{
		EnvironmentSettingsView->SetObject(Settings);
	}
	
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			EnvironmentSettingsView.ToSharedRef()
		];
}

void FUnrealAgentsEditorModule::EnvironmentSettingsButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(EnvironmentSettingsTabName);
}

void FUnrealAgentsEditorModule::RegisterMenus()
{
	FToolMenuInsert InsertPos(NAME_None, EToolMenuInsertType::Default);
	
	FToolMenuOwnerScoped OwnerScoped(this);
	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("LevelEditor");
			Section.AddMenuEntryWithCommandList(FUnrealAgentsEditorCommands::Get().OpenEnvironmentSettingsWindow, PluginCommands).InsertPosition = InsertPos;
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.LevelToolbarQuickSettings");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("ProjectSettingsSection");
			{
				Section.AddMenuEntryWithCommandList(FUnrealAgentsEditorCommands::Get().OpenEnvironmentSettingsWindow, PluginCommands);
			}
		}
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FUnrealAgentsEditorModule, UnrealAgentsEditor)