using UnrealBuildTool;

public class UnrealAgentsEditor : ModuleRules
{
	public UnrealAgentsEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(
			new string[] {
            }
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
                "UnrealAgentsCore/Private",
                "UnrealAgentsCore/Private/Metrics"
            }
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Projects",
				"InputCore",
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"UnrealEd",
                "ToolMenus",
				"PropertyEditor",
				"LevelEditor",
				"UnrealAgentsCore",
				"ThirdParty"
            }
			);
    }
}
