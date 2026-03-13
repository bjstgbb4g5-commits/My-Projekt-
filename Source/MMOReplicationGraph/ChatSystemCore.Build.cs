using UnrealBuildTool;

public class ChatSystemCore : ModuleRules
{
    public ChatSystemCore(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "NetCore",
            "PlayerRegistrySubsystem",
            "PartySystemCore",
            "GuildSystemCore"
        });
    }
}
