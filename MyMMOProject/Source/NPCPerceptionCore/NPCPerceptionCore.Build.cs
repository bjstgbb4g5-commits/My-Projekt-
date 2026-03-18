using UnrealBuildTool;

public class NPCPerceptionCore : ModuleRules
{
    public NPCPerceptionCore(ReadOnlyTargetRules Target) : base(Target)
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
            "NPCLifeSimulationCore",
            "NPCDensitySystemCore",
            "CityEventSystemCore"
        });
    }
}
