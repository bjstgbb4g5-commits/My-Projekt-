using UnrealBuildTool;

public class AIDirectorCore : ModuleRules
{
    public AIDirectorCore(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "NetCore",
            "AICoreSystem"
        });
    }
}
