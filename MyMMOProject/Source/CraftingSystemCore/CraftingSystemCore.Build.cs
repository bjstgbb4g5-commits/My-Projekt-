using UnrealBuildTool;

public class CraftingSystemCore : ModuleRules
{
    public CraftingSystemCore(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "NetCore",
            "InventorySystemCore",
            "ItemSystemCore",
            "WorldEconomyCore"
        });
    }
}
