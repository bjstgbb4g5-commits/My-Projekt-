using UnrealBuildTool;

public class CharacterProgressionCore : ModuleRules
{
    public CharacterProgressionCore(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "NetCore",
            "WorldGeneratorCore",
            "WorldEventCore",
            "EconomyCore",
            "CombatCore"
        });

        PrivateDependencyModuleNames.AddRange(new string[]
        {
            "Slate",
            "SlateCore"
        });
    }
}
