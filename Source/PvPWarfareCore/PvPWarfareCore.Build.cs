using UnrealBuildTool;

public class PvPWarfareCore : ModuleRules
{
    public PvPWarfareCore(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "NetCore",
            "WorldGeneratorCore",
            "WorldEconomyCore",
            "GuildSystemCore",
            "CityPoliticsCore",
            "MountSystemCore"
        });
    }
}
