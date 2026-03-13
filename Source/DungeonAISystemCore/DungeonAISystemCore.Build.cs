using UnrealBuildTool;

public class DungeonAISystemCore : ModuleRules
{
    public DungeonAISystemCore(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "AIDirectorCore",
            "AntiCheatSystemCore",
            "WorldBossSystemCore",
            "LiveEventSystemCore"
        });
    }
}
