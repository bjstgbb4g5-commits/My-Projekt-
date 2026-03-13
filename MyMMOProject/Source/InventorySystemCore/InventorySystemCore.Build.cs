using UnrealBuildTool;

public class InventorySystemCore : ModuleRules
{
    public InventorySystemCore(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "NetCore",
            "ItemSystemCore" // для EItemRarity и работы с предметами
        });
    }
}
