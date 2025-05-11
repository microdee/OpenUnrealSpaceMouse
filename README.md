# Open Unreal SpaceMouse
Previously "SpaceMouse for Unreal Engine" or "UE4-SpaceMouse". Name is changed to further differentiate this from 3DConnexion's own plugin. Old URL's for "UE4-SpaceMouse" still should work because how Github keeps old-name alive, but update your URL's just in case.

- [SpaceMouse for Unreal Engine](#spacemouse-for-unreal-engine)
- [Installation](#installation)
  - [Disclaimer about the new official plugin shipped by 3DConnexion](#disclaimer-about-the-new-official-plugin-shipped-by-3dconnexion)
  - [Requirements](#requirements)
- [Config / Usage](#config--usage)
  - [Buttons](#buttons)
    - [Default editor bindings](#default-editor-bindings)
  - [Behavior](#behavior)
    - [Orbiting](#orbiting)
    - [Orthographic viewports](#orthographic-viewports)
  - [Axis configuration](#axis-configuration)
  - [Misc](#misc)
- [More info](#more-info)
  - [Changelog](#changelog)
  - [Major changes from version 1.2](#major-changes-from-version-12)
  - [Major changes from version 1.1](#major-changes-from-version-11)
  - [Build a release](#build-a-release)
  - [Note on macOS support](#note-on-macos-support)
  - [Known issues:](#known-issues)
  - [Known nuances](#known-nuances)
  - [Credits](#credits)

# Installation
  
**Pre-Compiled for UE 4.23 .. 4.26.**

Demo video: https://youtu.be/owcSTzs8p-8

**[Download manually for free](https://github.com/microdee/UE4-SpaceMouse/releases/latest)**

**[Buy it via the Marketplace](https://www.unrealengine.com/marketplace/en-US/product/spacemouse-for-unreal-engine)**

**If you have a code project just use this repo as submodule**
```
git submodule add https://github.com/microdee/UE4-SpaceMouse.git Plugins/SpaceMouse
```

## [Disclaimer about the new official plugin shipped by 3DConnexion](DISCLAIMER.md)

## Requirements

This plugin works with the HID device directly and doesn't use 3DxWare (official "drivers") SDK or its services (similar how Blender does it). However it has different requirements depending on your operating system:

* **Windows** treats space mice as generic HID input devices before 3DxWare is installed. Therefore this plugin works out of the box, without the need for official drivers to be installed.
  * In case you need to install their software package for other applications, make sure to untick their UE4 plugin in the installer.
  * However if you already installed the official drivers with default settings, please remove their UE4 plugin from `<path-to-ue4>\Engine\Plugins\3DConnexion` before using this one.
  * Supporting official 3DxWare shenanigans are a planned thing now on Windows as well, when the driver is installed.
* **MacOS** [see below](#note-on-macos-support)

# Config / Usage

You can control any 3D editor viewport (Perspective or Ortho) currently in focus (user needs to click on it first).

![Alt text](/ReadmeMedia/featured.png?raw=true "Coordinates")

Transition speed is controllable via the camera speed setting and the camera speed scalar of the viewport. Camera speed can be increased or decreased with configurable buttons on the spacemice. Multiple devices are supported however their data is merged together into a global context.

On default speeds (speed setting = 4) transition velocity is 1000 units/sec by default and rotation speed is 270°/sec by default. Rotation speed is not affected by camera speed settings. You can change these in the **Editor Preferences -> Plugins -> SpaceMouse** section.

![Alt text](/ReadmeMedia/settings.png?raw=true "Settings")

## Buttons

With **Button mappings** one can remap some functionalities to SpaceMouse buttons.
* For the SpaceMouse specific actions listed in here, either explicitly select a button, or press the small "keyboard" icon to learn a button.
* For further actions go to the `Keyboard Shortcuts` Editor preferences (link provided) and bind your SpaceMouse buttons as you would bind a regular keyboard shortcut. Using the ***SECONDARY*** input binding form is recommended.

### Default editor bindings

There's a pre-defined default set of bindings you can activate by clicking on _"Configure default button bindings"_.

* Under the hood it just assigns sensically the buttons from space mice, to the ***SECONDARY*** binding of the equivalent or close-enough™ Unreal Editor input action. It doesn't touch the primary binding which is usually the default keyboard shortcut.
* In some rare cases there's already something assigned as the ***SECONDARY*** binding which will be overwritten by this operation. This is also the reason why I chose to trigger this by the user, and why it's guarded with a confirmation dialog.

## Behavior

**Active in Background** When false, only control the camera of UE4 while the editor is the foreground application.

**Camera Behavior** Sets the mode of controlling the viewport camera. All behavior controls the viewport camera.

* **Camera Delta with Roll**
  * SpaceMouse represents the Delta Movement of the camera in Free-flight/Helicopter fashion. All rotation is in current camera space.
* **Camera Delta with No Roll**
  * Same as above but roll is ignored and Yaw is always in World space (instead of current camera space))
* **Orbiting with Roll**
  * SpaceMouse represents the Delta Movement of the camera around "the object" OR the Delta movement of "the object" in front of the camera. The pivot of orbiting is either surface point in the center of the viewport, or a fixed distance if set in the settings.
* **Orbiting with No Roll**
  * Same as above but roll is ignored and Yaw is always in World space (instead of current camera space))

### Orbiting

**Orbiting Moves/Rotates Object** When true, SpaceMouse represents the Delta movement of "the object" in front of the camera (meaning axes are inverted)

**Orbiting at Fixed Pivot Distance** When enabled, sets a fixed distance for the orbiting mode in UE4 units (usually cm).

**Orbiting Line Trace Length** is the maximum possible radius of the orbiting behavior in UE4 units (usually cm).

### Orthographic viewports

Camera controls are slightly different when controlling an Orthographic viewport (Top/Bottom, Left/Right, Front/Rear). Rotation is disabled, and the SpaceMouse is used only to pan on the selected plane and zoom.

**Ortho Zoom Speed** An arbitrary multiplier of the Zoom speed while insdide an Orthographic viewport. Negative numbers reverse the direction of zooming.

**OrthoPanningPlane** Select the plane of the device (2 axes) which will control the panning motion inside an Orthographic viewport.
* **Lateral Is Zoom, Vertical Is Up**: Move puck orthogonal to the plane of the Monitor to move around, Push Forward / Pull Backward to zoom.
* **Lateral Is Up, Vertical Is Zoom**: Move puck orthogonal to the plane of the Desk to move around, Push Down / Pull Up to zoom.

## Axis configuration

**Axis mappings** are conversions between the space of spacemice and the space of UE4. You can invert rotations and translations here by flipping the sign. The default values are my subjective preferences (as you can see in the coordinate system image above).

**Rotation/Translation Curve** are defining the 0..1 curvature from resting state to fully pushed/pulled state of the SpaceMouse puck. Dead-zone, clamping, exponent or other less useful transformations can be expressed with this. Default is 0..1 linear curve, which means no effect.

## Misc

**Display Debug Information** prints HID data onto the viewport. You don't need to change **Max Hid Read Operations Per Frame**. Just ignore it.

[Report bugs](https://github.com/microdee/UE4-SpaceMouse/issues)

[Have any feedback or ideas? Go to Discussions!](https://github.com/microdee/UE4-SpaceMouse/discussions)

[UE4 forum (please prefer Github Discussions)](https://forums.unrealengine.com/unreal-engine/feedback-for-epic/437-support-for-space-navigator-3d-mouse?p=1609440#post1609440)

# More info

## [Changelog](CHANGELOG.md)

## Major changes from version 1.2

The structure of SpaceMouse preferences has changed significantly. Upon updating from prior versions (<= 1.1.x) you might need to re-configure the plugin. Don't worry though, the button binding UX has been improved significantly.

## Major changes from version 1.1

Upon updating from prior versions (<= 1.0.x pre-marketplace) please delete both HIDUE and SpaceMouse plugins from your Engine or Project plugins folder!

1. Editor settings is moved to Plugins -> SpaceMouse to follow conventions
2. HIDUE is now demoted to a module instead of an entire plugin
3. This repository now only contains the plugin itself which you can also directly submodule now in your code projects if you wish.

## Build a release

If for any reason you cannot use this plugin as a submodule and you want to make a pre-built release to be an engine plugin, then work with the [official test project](https://github.com/microdee/UE4-SpaceMouse-TestProject) and see the build instructions there.

## Build a release for Linux

To make a pre-built release for Linux, read the [Linux Build Guide](LINUX.md).

## Note on macOS support

3DConnexion claims to have Mac support for their official plugin now. In light of that Mac support is no longer persued and if you want to use your Space Mouse on Mac with Unreal Engine you should install their official plugin. This plugin keeps support for Windows and Linux (reportedly).

## Known issues:

* ~~"Focus selected" is broken while this plugin is loaded~~
  * FIXED
* ~~"Piloting" is not working with the SpaceMouse~~
  * FIXED
* ~~Upon exiting the editor the engine crashes. I know why it does that but I didn't figure it out quite yet how to prevent it.~~
  * FIXED
* Jumping forward periodically when leaning / going forward with the camera
  * __TL;DR:__ Open 3DxWare per-application settings and disable scroll-wheel emulation (disable tilt axis) (per-application settings are opened by the Menu button or bottom left button on your SpaceMouse by default)
  * [See issue](https://github.com/microdee/UE4-SpaceMouse/issues/12)
* If you install or update 3DxWare with their new plugin (TDxUnrealEditor), and you had this plugin before, you might experience undefined interaction glitches.
  * Remove either this or the 3DConnexion plugin (TDxUnrealEditor) according to your preference.

## Known nuances
* ~~Only free-fly mode is supported and every orbiting perspective viewport in focus is forced to be free-fly while any spacemice operates on them.~~
  * Now multiple camera behaviors are implemented
* Camera roll is not reset when one tries to navigate viewport with traditional methods
  * There's a mappable button now to reset roll
  * There are camera behaviors now which should not introduce roll
  * In case the SpaceMouse becomes unavailable Maya style Alt + Right-drag dollying resets camera roll

## Credits

* [HIDAPI](https://github.com/libusb/hidapi)
* **Gabor Papp** for initially investigating macOS support
* From UE4 forums:
  * **enzo_molinari** for the plugin icon
  * **adlord** for testing with wireless devices
  * **mrboni** for testing with discontinued SpacePilot Pro
