using UnrealBuildTool;

public class PvEWorldCore : ModuleRules
{
    public PvEWorldCore(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "AICoreSystem",
            "CombatCore",
            "CharacterProgressionCore",
            "WorldGeneratorCore",
            "WorldEconomyCore",
            "NavigationSystem"
        });
    }
}
