using Nuke.Common;
using Nuke.Common.IO;
using Nuke.Common.Tooling;
using Nuke.Cola;
using Nuke.Cola.BuildPlugins;
using Nuke.Unreal;
using Serilog;
using System;
using Nuke.Cola.FolderComposition;
using System.Security.Cryptography;

public static class UseOpenUnrealSpaceMouseGraph
{
    public static ITargetDefinition OpenUnrealSpaceMouseGraph(this ITargetDefinition target) => target
        .After<IMcroLicenseRegion>(_ => _.EnsureMcroLicense, _ => _.RenderMcroAttribution)
        .After<IUseYamlCpp>()
        .After<IUseRangeV3>()
    ;
}

[ImplicitBuildInterface]
public interface IUseOpenUnrealSpaceMouse : INukeBuild
{
    Target GenerateOpenUnrealSpaceMouseDocs => _ => _
        .Triggers<IOpenUnrealSpaceMouseLicenseRegion>(_ => _.RenderOpenUnrealSpaceMouseAttribution)
        .Executes(() =>
        {
            ToolResolver.GetPathTool("doxygen")(
                (this.ScriptFolder() / "Doxyfile").ToString(),
                workingDirectory: this.ScriptFolder()
            );
        });
}