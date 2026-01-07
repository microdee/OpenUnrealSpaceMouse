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
using Nuke.Unreal.Plugins;

[ImplicitBuildInterface]
public interface IUseOpenUnrealSpaceMouse : INukeBuild
{
    Target BuildOuesm => _ => _
        .Executes(() =>
        {
            var self = (UnrealBuild)this;
            var thisPlugin = UnrealPlugin.Get(this.ScriptFolder());
            var output = thisPlugin.BuildPlugin(self);
            var deploy = output.Parent / "Deploy";
            deploy.CreateOrCleanDirectory();
            var pluginsOutputDir = self.GetOutput() / "Plugins";

            foreach (var plugin in pluginsOutputDir.GetDirectories())
            {
                Log.Information("Deploying {0}", plugin.Name);
                (plugin / "Build").Copy(deploy / plugin.Name, ExistsPolicy.MergeAndOverwrite);
            }
            var archives = self.ProjectFolder / ".deploy";
            var zipName = $"{thisPlugin.Name}-{self.Platform}-V.{thisPlugin.Version}-UE.{Unreal.Version(self).VersionPatch}";

            Log.Information("Archiving for distribution");
            deploy.ZipTo(
                archives / $"{zipName}.zip",
                f => !f.HasExtension(".pdb") && !f.HasExtension(".exp")
            );

            Log.Information("Archiving debug symbols");
            deploy.ZipTo(
                archives / $"{zipName}-DebugSymbols.zip",
                f => f.HasExtension(".pdb") || f.HasExtension(".exp")
            );

            Log.Information("Archiving distributable source");
            var distribution = thisPlugin.GetDistributionOutput(self);
            distribution.ZipTo(archives / $"{zipName}-Source.zip");
        });
}