using UnrealBuildTool;
using System.Collections.Generic;

public class MyMMOProjectTarget : TargetRules
{
    public MyMMOProjectTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.V5;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_5;
        ExtraModuleNames.AddRange(new string[] { "MyMMOProject",
    "WorldSimulationCore"});
    }
}
