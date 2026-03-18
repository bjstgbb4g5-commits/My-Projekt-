
using UnrealBuildTool;

public class SocialSimulationCore : ModuleRules
{
	public SocialSimulationCore(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"AIModule",
			"NPCMemoryCore"
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
		});
	}
}
