using UnrealBuildTool;

public class MarketplaceSystemCore : ModuleRules
{
    public MarketplaceSystemCore(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "NetCore",
            "WorldEconomyCore",
            "GuildSystemCore",
            "CityPoliticsCore",
            "PvPWarfareCore",
            "ChronicleSystemCore"
        });
    }
}
