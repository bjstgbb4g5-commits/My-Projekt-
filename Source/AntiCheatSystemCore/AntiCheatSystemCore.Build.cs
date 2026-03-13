using UnrealBuildTool;

public class AntiCheatSystemCore : ModuleRules
{
    public AntiCheatSystemCore(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "NetCore",
            "Json",
            "JsonUtilities"
        });
    }
}
