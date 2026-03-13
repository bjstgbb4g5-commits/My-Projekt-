using UnrealBuildTool;

public class ResourceEconomySystemCore : ModuleRules
{
    public ResourceEconomySystemCore(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "NetCore",
            "MarketplaceSystemCore",
            "WorldTerritorySystemCore",
            "AIDirectorCore"
        });
    }
}
