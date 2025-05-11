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

public class SpaceMouseRuntime : ModuleRules
{
    public SpaceMouseRuntime(ReadOnlyTargetRules Target) : base(Target)
    {
        // C++23
        bUseUnity = false;
        CppStandard = CppStandardVersion.Latest;
        bEnableUndefinedIdentifierWarnings = false;

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
