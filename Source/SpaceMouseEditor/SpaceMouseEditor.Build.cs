// Copyright 2018-2021 David Morasz All Rights Reserved.
// This source code is under MIT License https://github.com/microdee/UE4-SpaceMouse/blob/master/LICENSE

using UnrealBuildTool;

public class SpaceMouseEditor : ModuleRules
{
    public SpaceMouseEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.NoPCHs;
        bEnableUndefinedIdentifierWarnings = false;

        IsRedistributableOverride = true;;
        bLegalToDistributeObjectCode = true;
        bPrecompile = true;
        PrecompileForTargets = PrecompileTargetsType.Any;
        
        PublicDependencyModuleNames.AddRange(new []
        {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "PropertyEditor",

            "SpaceMouseReader",
            "SpaceMouseRuntime"
        });

        if (Target.Type == TargetType.Editor)
        {
            PublicDependencyModuleNames.AddRange(new []
            {
                "Slate",
                "SlateCore",
                "UnrealEd",
                "EditorStyle",
                "DetailCustomizations",
                "LevelEditor",
                "ApplicationCore",
                "RHI",
                "GraphEditor"
            });
        }
    }
}
