using System;
using System.IO;
using UnrealBuildTool;

public class ThirdParty : ModuleRules
{
    public ThirdParty(ReadOnlyTargetRules Target) : base(Target)
    {
        Type = ModuleType.External;

        PublicIncludePaths.AddRange(
            new string[] {
				Path.Combine(ModuleDirectory, "Oatpp", "include"),
                Path.Combine(ModuleDirectory, "Oatpp-websocket", "include"),
                Path.Combine(ModuleDirectory, "Pybind", "include")
            }
        );

        PublicAdditionalLibraries.AddRange(
            new string[] {
                Path.Combine(ModuleDirectory, "Oatpp", "oatpp.lib"),
                Path.Combine(ModuleDirectory, "Oatpp-websocket", "oatpp-websocket.lib")
            }
        );
    }
}