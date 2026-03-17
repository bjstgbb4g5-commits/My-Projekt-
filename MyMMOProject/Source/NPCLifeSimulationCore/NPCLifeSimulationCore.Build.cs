using UnrealBuildTool;

public class NPCLifeSimulationCore : ModuleRules
{
    public NPCLifeSimulationCore(ReadOnlyTargetRules Target) : base(Target)
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
            "WorldSimulationCore",
            "AIDirectorCore"
        });
    }
}
