using UnrealBuildTool;

public class LootSystemCore : ModuleRules
{
    public LootSystemCore(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "NetCore",
            "InventorySystemCore",
            "AIDirectorCore",
            "WorldBossSystemCore",
            "DungeonSystemCore"
        });
    }
}
