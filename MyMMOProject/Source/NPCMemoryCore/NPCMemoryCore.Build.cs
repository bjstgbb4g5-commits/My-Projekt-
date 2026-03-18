
using UnrealBuildTool;

public class NPCMemoryCore : ModuleRules
{
	public NPCMemoryCore(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"AIModule"
		});
	}
}
