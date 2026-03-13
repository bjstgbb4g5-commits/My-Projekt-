using UnrealBuildTool;

public class DungeonSystemCore : ModuleRules
{
    public DungeonSystemCore(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "AICoreSystem",
            "PvEWorldCore",
            "CombatCore",
            "WorldEconomyCore"
        });
    }
}
