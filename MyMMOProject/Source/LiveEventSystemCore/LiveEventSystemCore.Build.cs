using UnrealBuildTool;

public class LiveEventSystemCore : ModuleRules
{
    public LiveEventSystemCore(ReadOnlyTargetRules Target) : base(Target)
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
