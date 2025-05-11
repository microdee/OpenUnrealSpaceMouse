using Nuke.Common;
using Nuke.Common.IO;
using Nuke.Cola;
using Nuke.Cola.BuildPlugins;
using Serilog;
using Nuke.Common.Utilities;
using YamlDotNet.Serialization;
using Nuke.Utilities.Text.Yaml;
using System.Linq;

public class ThirdPartyAttribution
{
    [YamlMember(Alias = "name")]
    public string? Name;

    [YamlMember(Alias = "source")]
    public string? Source;

    [YamlMember(Alias = "project")]
    public string? Project;

    [YamlMember(Alias = "authors")]
    public string[] Authors = [];

    [YamlMember(Alias = "license")]
    public string? License;

    [YamlMember(Alias = "reasoning")]
    public string? Reasoning;
}

[ImplicitBuildInterface]
public interface IOpenUnrealSpaceMouseLicenseRegion : INukeBuild
{
    Target EnsureOpenUnrealSpaceMouseLicense => _ => _
        .Executes(() =>
        {
            this.ProcessLicenseRegion(
                this.ScriptFolder(),
                new(
                    """
                    This Source Code is subject to the terms of the Mozilla Public License, v2.0.
                    If a copy of the MPL was not distributed with this file You can obtain one at
                    https://mozilla.org/MPL/2.0/
                    """,
                    "David Mórász", 2025
                ),
                new() {
                    AllowDirectory = d => !d.Name.EqualsAnyOrdinalIgnoreCase(
                        "ThirdParty"
                    ),
                    AllowFile = f => !f.Name.EqualsAnyOrdinalIgnoreCase(
                        "IspcParallelism.cpp"   
                    )
                }
            );
        });

    Target RenderOpenUnrealSpaceMouseAttribution => _ => _
        .Executes(() =>
        {
            var target = this.ScriptFolder() / "Docs" / "Manual" / "ATTRIBUTION.md";
            target.WriteAllText("# Attribution {#Attribution}");
            foreach (var file in this.ScriptFolder().GetFiles("*.tp.yml", 40))
            {
                Log.Debug("Including {0}", file);
                var attribution = file.ReadYaml<ThirdPartyAttribution>();
                var authors = attribution.Authors.Select(a => "  * " + a).JoinNewLine();
                var markdown =
                    $"""
                    
                    ## [{attribution.Name}]({attribution.Project})

                    * Authors:
                    {authors}
                    * License: {attribution.License}
                    * [Source code]({attribution.Source})

                    Reason of inclusion:  
                    {attribution.Reasoning}

                    """;
                target.AppendAllText(markdown);
            }
        });
}