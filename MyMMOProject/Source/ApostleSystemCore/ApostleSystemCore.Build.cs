using UnrealBuildTool;

public class ApostleSystemCore : ModuleRules
{
    public ApostleSystemCore(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "AICoreSystem",
            "CombatCore",
            "PvEWorldCore",
            "WorldBossSystemCore"
        });
    }
}
