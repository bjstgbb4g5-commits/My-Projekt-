using UnrealBuildTool;

public class MountSystemCore : ModuleRules
{
    public MountSystemCore(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "NetCore",
            "WorldGeneratorCore",
            "CharacterProgressionCore",
            "WorldEconomyCore"
        });
    }
}
