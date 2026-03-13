using UnrealBuildTool;

public class WorldTerritorySystemCore : ModuleRules
{
    public WorldTerritorySystemCore(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "NetCore",
            "GuildSystemCore",
            "MarketplaceSystemCore",
            "PvPWarfareCore",
            "AIDirectorCore"
        });
    }
}
