// Copyright 2018-2021 David Morasz All Rights Reserved.
// This source code is under MIT License https://github.com/microdee/UE4-SpaceMouse/blob/master/LICENSE

using UnrealBuildTool;

public class SpaceMouseRuntime : ModuleRules
{
    public SpaceMouseRuntime(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.NoPCHs;
        bEnableUndefinedIdentifierWarnings = false;

        IsRedistributableOverride = true;
        bLegalToDistributeObjectCode = true;
        bPrecompile = true;
        PrecompileForTargets = PrecompileTargetsType.Any;

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
