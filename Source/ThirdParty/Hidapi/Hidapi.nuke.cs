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

using Nuke.Common;
using Nuke.Common.IO;
using Nuke.Common.Tooling;
using Nuke.Cola;
using Nuke.Cola.Tooling;
using Nuke.Cola.BuildPlugins;
using Nuke.Unreal;
using Nuke.Unreal.BoilerplateGenerators.XRepo;
using Serilog;
using System;

[ImplicitBuildInterface]
[ParameterPrefix("hidapi")]
public interface IUseHidapi : INukeBuild
{
    AbsolutePath HidapiInputDir => this.ScriptFolder() / "hidapi";

    string[] HidapiConfigs => [ "Release", "Debug" ];

    Target PrepareHidapi => _ => _
        .DependentFor<UnrealBuild>(b => b.Prepare)
        .Executes(() =>
        {
            ((UnrealBuild) this).InstallXRepoLibrary(
                "hidapi 0.14.0", "", this.ScriptFolder()
            );
        });
}