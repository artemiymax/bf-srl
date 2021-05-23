using UnrealBuildTool;

public class UnrealAgentsCore : ModuleRules
{
    public UnrealAgentsCore(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        bEnableExceptions = true;

        PublicIncludePaths.AddRange(
            new string[] {
                "C:/Program Files/Python38/include"
            }
            );


        PrivateIncludePaths.AddRange(
            new string[] {
                "UnrealAgentsCore/Private",
                "UnrealAgentsCore/Private/Metrics",
                "UnrealAgentsCore/Private/CommandServer",
                "UnrealAgentsCore/Private/CommandServer/Controllers",
                "UnrealAgentsCore/Private/CommandServer/Dto"
            }
            );


        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "Networking"
            }
            );

        if (Target.Type == TargetType.Editor)
        {
            PrivateDependencyModuleNames.Add("UnrealEd");
        }

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "PhysX",
                "ThirdParty"
            }
            );

        PublicAdditionalLibraries.AddRange(
            new string[] {
                "C:/Program Files/Python38/libs/python38.lib"
            }
        );
    }
}
