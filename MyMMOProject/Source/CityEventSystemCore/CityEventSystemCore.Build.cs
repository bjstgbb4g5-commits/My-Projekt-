using UnrealBuildTool;

public class CityEventSystemCore : ModuleRules
{
    public CityEventSystemCore(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine"
        });

        PrivateDependencyModuleNames.AddRange(new string[]
        {
            "WorldSimulationCore",
            "NPCLifeSimulationCore",
            "NPCDensitySystemCore",
            "NPCActivitySystemCore",
            "AIDirectorCore"
        });
    }
}
