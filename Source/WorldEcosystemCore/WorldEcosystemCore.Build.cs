using UnrealBuildTool;

public class WorldEcosystemCore : ModuleRules
{
    public WorldEcosystemCore(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "AICoreSystem",
            "PvEWorldCore",
            "NavigationSystem"
        });
    }
}
