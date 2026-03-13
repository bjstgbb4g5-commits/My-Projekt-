using UnrealBuildTool;

public class OlympiadSystemCore : ModuleRules
{
    public OlympiadSystemCore(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "NetCore",
            "ChronicleSystemCore",
            "PvPWarfareCore",
            "CharacterProgressionCore",
            "GuildSystemCore"
        });
    }
}
