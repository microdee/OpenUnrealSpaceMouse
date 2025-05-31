// Fill in Copyright info...

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