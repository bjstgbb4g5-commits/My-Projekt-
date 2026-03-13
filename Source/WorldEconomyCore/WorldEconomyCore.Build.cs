using UnrealBuildTool;

public class WorldEconomyCore : ModuleRules
{
    public WorldEconomyCore(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "NetCore",
            "WorldGeneratorCore",
            "WorldPopulationCore",
            "WorldEventCore"
        });
    }
}
