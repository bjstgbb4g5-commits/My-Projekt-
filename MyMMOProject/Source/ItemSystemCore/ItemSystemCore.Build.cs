using UnrealBuildTool;

public class ItemSystemCore : ModuleRules
{
    public ItemSystemCore(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
         }
       );
    }
}
