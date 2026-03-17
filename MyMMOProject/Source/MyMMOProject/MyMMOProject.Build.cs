using UnrealBuildTool;

public class MyMMOProject : ModuleRules
{
    public MyMMOProject(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
{
         "Core",
         "CoreUObject",
         "Engine",
         "InputCore",
         "DeveloperSettings"
});

        PrivateDependencyModuleNames.AddRange(new string[]
        {

        });
    }
}
