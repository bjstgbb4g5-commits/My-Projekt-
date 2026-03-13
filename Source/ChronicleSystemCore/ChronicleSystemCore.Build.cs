using UnrealBuildTool;

public class ChronicleSystemCore : ModuleRules
{
    public ChronicleSystemCore(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "NetCore",
            "PvEWorldCore",
            "PvPWarfareCore",
            "GuildSystemCore",
            "WorldEconomyCore",
            "CityPoliticsCore"
        });
    }
}
