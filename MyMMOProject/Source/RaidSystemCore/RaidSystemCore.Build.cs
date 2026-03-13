using UnrealBuildTool;

public class RaidSystemCore : ModuleRules
{
    public RaidSystemCore(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "AICoreSystem",
            "CombatCore",
            "DungeonSystemCore",
            "PvEWorldCore"
        });
    }
}
