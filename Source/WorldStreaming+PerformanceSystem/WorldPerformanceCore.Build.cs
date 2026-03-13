using UnrealBuildTool;

public class WorldPerformanceCore : ModuleRules
{
    public WorldPerformanceCore(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "NetCore",
            "NavigationSystem",
            "AIModule",
            "WorldGeneratorCore",
            "WorldPopulationCore"
        });
    }
}
