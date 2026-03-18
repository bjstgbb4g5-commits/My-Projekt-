
using UnrealBuildTool;

public class UtilityAICore : ModuleRules
{
    public UtilityAICore(ReadOnlyTargetRules Target) : base(Target)
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
            "NPCPerceptionCore",
            "NPCLifeSimulationCore",
            "NPCActivitySystemCore",
            "CityEventSystemCore"
        });
    }
}
