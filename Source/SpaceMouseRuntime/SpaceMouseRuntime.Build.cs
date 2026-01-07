// Copyright 2018-2021 David Morasz All Rights Reserved.
// This source code is under MIT License https://github.com/microdee/UE4-SpaceMouse/blob/master/LICENSE

using UnrealBuildTool;

public class SpaceMouseRuntime : ModuleRules
{
    public SpaceMouseRuntime(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.NoPCHs;
#if UE_5_6_OR_LATER
        CppCompileWarningSettings.UndefinedIdentifierWarningLevel = WarningLevel.Off;
#elif UE_5_5_OR_LATER
        UndefinedIdentifierWarningLevel = WarningLevel.Off;
#else
        bEnableUndefinedIdentifierWarnings = false;
#endif

        if (Target.Version.MajorVersion >= 5)
        {
            PrivateDependencyModuleNames.AddRange(new [] {
                "ApplicationCore"
            });
        }
            
        
        PublicDependencyModuleNames.AddRange(new []
        {
            "Core",
            "CoreUObject",
            "Engine",

            "SpaceMouseReader",
            "InputCore",
            "InputDevice"
        });
    }
}
