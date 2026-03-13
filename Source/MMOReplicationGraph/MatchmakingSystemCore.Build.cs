using UnrealBuildTool;

public class MatchmakingSystemCore : ModuleRules
{
    public MatchmakingSystemCore(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "NetCore",
            "PlayerRegistrySubsystem"
        });
    }
}
