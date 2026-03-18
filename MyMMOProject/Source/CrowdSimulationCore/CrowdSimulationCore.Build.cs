
using UnrealBuildTool;

public class CrowdSimulationCore : ModuleRules
{
    public CrowdSimulationCore(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "AIModule"
        });

        PrivateDependencyModuleNames.AddRange(new string[]
        {
            "NavigationSystem",
            "NPCLifeSimulationCore",
            "NPCActivitySystemCore",
            "NPCDensitySystemCore",
            "UtilityAICore"
        });
    }
}
