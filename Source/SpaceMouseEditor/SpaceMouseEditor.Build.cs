/** @noop License Comment
 *  @file
 *  @copyright
 *  This Source Code is subject to the terms of the Mozilla Public License, v2.0.
 *  If a copy of the MPL was not distributed with this file You can obtain one at
 *  https://mozilla.org/MPL/2.0/
 *  
 *  @author David Mórász
 *  @date 2025
 */



using UnrealBuildTool;

public class SpaceMouseEditor : ModuleRules
{
    public SpaceMouseEditor(ReadOnlyTargetRules Target) : base(Target)
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
                "RHI"
            });
        }
    }
}
