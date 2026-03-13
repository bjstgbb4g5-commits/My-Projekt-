using UnrealBuildTool;

public class WorldBossSystemCore : ModuleRules
{
    public WorldBossSystemCore(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "AICoreSystem",
            "CombatCore",
            "PvEWorldCore"
        });
    }
}
