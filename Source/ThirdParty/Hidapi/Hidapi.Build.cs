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

using System;
using UnrealBuildTool;

public partial class Hidapi : ModuleRules
{
	bool IsDebug;
	bool PlatformSetup = false;
	bool IncludesSetup = false;
	string LibraryConfig;
	string LibraryPlatform;

	public Hidapi(ReadOnlyTargetRules target) : base(target)
	{
		// This is the main module file for using hidapi in Unreal Engine.
		// Because each individual platform can carry different requirements for hidapi,
		// includes and lib imports are expressed in platform specific partial classes. These are
		// auto generated when the library is prepared on/for a specific platform.

		Type = ModuleType.External;
		IsDebug = target.Configuration == UnrealTargetConfiguration.Debug && target.bDebugBuildsActuallyUseDebugCRT;
		LibraryConfig = IsDebug ? "Debug" : "Release";
		LibraryPlatform = target.Platform.ToString();

		// Write user defined setup here

		// Set up library headers
		SetupLibrary_Includes(target);

		// Optional platform specific library setup:
		
				if (target.Platform == UnrealTargetPlatform.Win64)
					SetupLibrary_Win64(target);
				
				if (target.Platform == UnrealTargetPlatform.Mac)
					SetupLibrary_Mac(target);
				
				if (target.Platform == UnrealTargetPlatform.Linux)
					SetupLibrary_Linux(target);
				
				if (target.Platform == UnrealTargetPlatform.LinuxArm64)
					SetupLibrary_LinuxArm64(target);
				
				if (target.Platform == UnrealTargetPlatform.Android)
					SetupLibrary_Android(target);
				
				if (target.Platform == UnrealTargetPlatform.IOS)
					SetupLibrary_IOS(target);
				
				if (target.Platform == UnrealTargetPlatform.TVOS)
					SetupLibrary_TVOS(target);
				
#if UE_5_4_OR_LATER
				if (target.Platform == UnrealTargetPlatform.VisionOS)
					SetupLibrary_VisionOS(target);
#endif
				
		if (!PlatformSetup || !IncludesSetup)
		{
			throw new Exception(
				$"hidapi was not set up for {target.Platform}."
				+ $"\n Did you install the library for {target.Platform}? Run:"
				+ $"\n > nuke PrepareHidapi --platform {target.Platform}"
			);
		}
	}
	
	partial void SetupLibrary_Includes(ReadOnlyTargetRules target);
	partial void SetupLibrary_Win64(ReadOnlyTargetRules target);
	partial void SetupLibrary_Mac(ReadOnlyTargetRules target);
	partial void SetupLibrary_Linux(ReadOnlyTargetRules target);
	partial void SetupLibrary_LinuxArm64(ReadOnlyTargetRules target);
	partial void SetupLibrary_Android(ReadOnlyTargetRules target);
	partial void SetupLibrary_IOS(ReadOnlyTargetRules target);
	partial void SetupLibrary_TVOS(ReadOnlyTargetRules target);
	partial void SetupLibrary_VisionOS(ReadOnlyTargetRules target);
}