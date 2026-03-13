using UnrealBuildTool;

public class AICoreSystem : ModuleRules
{
    public AICoreSystem(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "AIModule",
            "GameplayTasks",
            "NavigationSystem",
            "CombatCore",
            "CharacterProgressionCore"
        });
    }
}
