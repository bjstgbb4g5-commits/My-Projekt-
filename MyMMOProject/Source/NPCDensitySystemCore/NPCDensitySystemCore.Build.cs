using UnrealBuildTool;

public class NPCDensitySystemCore : ModuleRules
{
    public NPCDensitySystemCore(ReadOnlyTargetRules Target) : base(Target)
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
            "WorldSimulationCore"
        });
    }
}
