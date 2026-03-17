using UnrealBuildTool;
using System.Collections.Generic;

public class MyMMOProjectEditorTarget : TargetRules
{
    public MyMMOProjectEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.V5;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_5;
        ExtraModuleNames.AddRange(new string[] { "MyMMOProject",
    "WorldSimulationCore"});
    }
}
