using UnrealBuildTool;

public class WorldSimulationCore : ModuleRules
{
    public WorldSimulationCore(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "NetCore"
        });

        PrivateDependencyModuleNames.AddRange(new string[]
        {
            "WorldEconomyCore",
            "WorldEventCore",
            "WorldPopulationCore",
            "AIDirectorCore"
        });
    }
}
