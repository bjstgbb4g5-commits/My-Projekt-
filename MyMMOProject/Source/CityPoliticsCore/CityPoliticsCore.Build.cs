using UnrealBuildTool;

public class CityPoliticsCore : ModuleRules
{
    public CityPoliticsCore(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "NetCore"
        });
    }
}
