## Disclaimer about the relationship to the new official plugin provided by 3DConnexion

After I released this plugin to the marketplace it came to my knowledge that 3D connexion also released their own plugin for UE4 editor (dubbed `TDxUnrealEditor`). I have no affiliation with 3DConnexion and 3DConnexion of course were not involved in any way during the development of this plugin (Open Unreal SpaceMouse). We had no knowledge about each other's work regarding the Unreal Engine implementation.

TDxUnrealEditor is installed automatically when you install or update the official drivers. Functionality-wise it matches this plugin. When both plugins are installed at the same time the behavior is undefined and I cannot guarantee that both would work as expected. Probably the simultaneous presence of both plugin would cause many interaction glitches. Here's how they compare to each-other:

|                                                   Feature |    TDxUnrealEditor     | Open Unreal SpaceMouse |
| --------------------------------------------------------: | :--------------------: | :--------------------: |
|                                               Open-Source | ✔ <sup>[1](#f1)</sup> |           ✔           |
|                       Unified 3DxWare features/navigation | ✔ <sup>[2](#f2)</sup> |           ❌           |
|                       Custom navigation scheme for Unreal |           ❌           |           ✔           |
| Control any 3D viewport in the editor <sup>[3](#f3)</sup> |           ✔           |           ✔           |
|    Control Blueprint and other graphs <sup>[8](#f8)</sup> |           ❌           |           ✔           |
|                                          Supports Windows |           ✔           |           ✔           |
|                                            Supports Linux |           ❌           |           ✔           |
|                                              Supports Mac |           ✔           |           ❌           |
|                               3DxWare bindings for Unreal |           ✔           |           ❌           |
|                 Button binding through editor preferences |           ❌           |           ✔           |
|                  Respect and set camera speed of viewport |           ❌           |           ✔           |
|          Automatic camera speed depending on surroundings |           ✔           |           ❌           |
|              Configurable deadzone and reaction curvature | ❌ <sup>[4](#f4)</sup> |           ✔           |
|                                   Control Ortho viewports | ❌ <sup>[7](#f7)</sup> |           ✔           |
|                         Piloting scene cameras and actors |           ✔           |           ✔           |
|                        Runtime support for packaged games |           ❌           | ✔ <sup>[6](#f6)</sup> |
|                                       Orbiting navigation |           ✔           |           ✔           |
|                                         Flying navigation |           ❌           |           ✔           |
|                          Keep Horizon ("Helicopter" mode) |           ✔           |           ✔           |
|                            Save views for recalling later |           ✔           |           ❌           |

Further differences

| TDxUnrealEditor                                         | Open Unreal SpaceMouse                               |
| ------------------------------------------------------- | ---------------------------------------------------- |
| Better orbit/object mode implementation                 | Orbit/object mode is more barebones                  |
| Configurable via 3DxWare                                | Configurable from Unreal Editor Preferences          |
| Fit selection to viewport is implemented by 3DConnexion | Uses vanilla Unreal fit to viewport function (F key) |

* This is not a complete list, and it might change with updates
* <a name="f1">1</a> the bridge code between Unreal and the 3DConnexion SDK is now source-available when installing through Fab
* <a name="f2">2</a> This also allows you to sync your preferences accross multiple applications, Open Unreal SpaceMouse obviously cannot provide or read that.
* <a name="f3">3</a> Including skeletal and static mesh editors, material preview, niagara editors, or really anything which provides a camera to control
* <a name="f4">4</a> Not to my knowledge anyway.
* <a name="f6">6</a> Both via Blueprint and the Input framework
* <a name="f7">7</a> Last time I've seen it didn't do that.
* <a name="f8">8</a> This include material editor, niagara systems, behavior trees, anything which has a graph of nodes
