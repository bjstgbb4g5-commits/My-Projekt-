using UnrealBuildTool;

public class QuestSystemCore : ModuleRules
{
    public QuestSystemCore(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "NetCore",
            "PlayerRegistrySubsystem",
            "PersistenceSystemCore",
            "EconomyCore",
            "ChronicleSystemCore",
            "WorldGeneratorCore",
            "CombatCore"
        });
    }
}
