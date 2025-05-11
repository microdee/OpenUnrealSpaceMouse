// Copyright 2018-2021 David Morasz All Rights Reserved.
// This source code is under MIT License https://github.com/microdee/UE4-SpaceMouse/blob/master/LICENSE

using UnrealBuildTool;
using System.IO;

public class SpaceMouseReader : ModuleRules
{
    public SpaceMouseReader(ReadOnlyTargetRules Target) : base(Target)
    {
        // C++23
        bUseUnity = false;
        CppStandard = CppStandardVersion.Latest;
        bEnableUndefinedIdentifierWarnings = false;
        
        PublicDependencyModuleNames.AddRange(new []
        {
            "Core",
            "CoreUObject",
            "Engine",

            "Hidapi"
            // ... add other public dependencies that you statically link with here ...
        });
    }
}
    
