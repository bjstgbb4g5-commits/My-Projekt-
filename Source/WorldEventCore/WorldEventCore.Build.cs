using UnrealBuildTool;

public class WorldEventCore : ModuleRules
{
    public WorldEventCore(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "NetCore",
            "ChronicleSystemCore",
            "PvEWorldCore",
            "PvPWarfareCore",
            "WorldEconomyCore",
            "CityPoliticsCore",
            "WorldGeneratorCore"
        });
    }
}
