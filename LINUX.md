# OpenUnrealSpaceMouse Build Guide for Linux

This guide will help you get up and running with the OpenUnrealSpaceMouse plugin on Linux.

## Compatibility

The installation process detailed in this guide has been validated to work on the following system configurations:

| Operating System | Engine                 |
|------------------|------------------------|
| Linux Mint 21.2  | UE 5.3+ (pre-compiled) |

## Prerequisites

This guide assumes that you've installed a compatible Unreal Engine version – however, it's likely that other versions will be fine as well. If the guide works for you with any other mix of OS and/or engine version, feel free to add your configuration to the compatibility table.

Unreal Engine is distributed with the necessary dependencies to package your games and build your plugins, however it's possible to use local installations of compilers and additional dependencies instead. **This guide assumes you're using the bundled toolchain**.

After installation, take note of your installation directory – you'll need it later.

If you intend to use your SpaceMouse for any other software you should consider installing the `spacenavd` driver (daemon) for additional support.

## Building

OpenUnrealSpaceMouse doesn't currently provide a pre-built plugin for Linux, so we have to build our own release before installation.

1. Clone this repository, clone [MCRO](https://mcro.de/mcro/), and install [Nuke.Unreal](https://mcro.de/Nuke.Unreal/) into your project

```sh
git clone --recurse-submodules https://github.com/microdee/OpenUnrealSpaceMouse.git
git clone --recurse-submodules https://github.com/microdee/mcro.git
dotnet tool install Nuke.GlobalTool --global
```

2. [Download the Nuke.Unreal bootstrap](https://github.com/microdee/Nuke.Unreal/releases/tag/Fresh) and extract it into your project root
3. Point nuke to your Unreal installation

```sh
nuke switch --Unreal ~/MyUnrealStuff/UE_5.6
```

4. Make an archived release

```sh
nuke BuildOuesm
```

5. Your release should show up in `.deploy` folder
6. Or alternatively you can just build your project with Nuke now (`prepare` only needs to be done once)

```sh
nuke prepare build-editor
```

Nuke.Unreal is not well tested on Linux but it should work, please report issues on Linux so I can support it better.

### Troubleshooting

When you try to build the plugin you might get an error saying `"libudev.h not found"`. If that's the case, install the `libudev-dev` package from your package manager and try again.

If you've installed local toolchains for C/C++ development using system packages such as `build-essential` (Debian, Ubuntu, etc.) you could experience issues with compiling the plugin. This was resolved by removing the installed packages, but for many that won't be a possibility. For now there's no documented way to resolve these issues.

## Installation

If you just want to use this in your project then you need to [prepare](https://mcro.de/Nuke.Unreal/d3/de9/Targets.html#target-prepare)/[generate](https://mcro.de/Nuke.Unreal/d3/de9/Targets.html#target-generate) your project with [Nuke.Unreal](https://mcro.de/Nuke.Unreal/) and then you can use them as usual.

If you want to make an engine plugin then you can use the archives which `BuildOuesm` has created.

Start the Unreal Editor and verify that the plugin is enabled in **Edit > Plugins > Input Devices**. Then check that plugin settings are available in **Edit > Editor Preferences > Plugins > SpaceMouse**.

Your're now ready to use your 3DConnexion hardware! Review the [README](README.md) for additional configuration options and how to use the plugin.

### Troubleshooting

After installation, if you can't use your SpaceMouse to navigate any editor viewports it's possible that your local user account doesn't have access to the HID device. This can be solved by adding the following `udev` rules ([source](https://github.com/prusa3d/PrusaSlicer/blob/master/resources/udev/90-3dconnexion.rules)) which should set permissions to allow any user read/write access to most 3DConnexion devices.

Copy the following content and save it in a file named `90-3dconnexion.rules` in the `/usr/lib/udev/rules.d` directory. **This will require root privileges!**

```
# 3D Connexion vendor devices
KERNEL=="hidraw*", ATTRS{idVendor}=="256f", ATTRS{idProduct}=="c603", MODE="0666"
KERNEL=="hidraw*", ATTRS{idVendor}=="256f", ATTRS{idProduct}=="c605", MODE="0666"
KERNEL=="hidraw*", ATTRS{idVendor}=="256f", ATTRS{idProduct}=="c606", MODE="0666"
KERNEL=="hidraw*", ATTRS{idVendor}=="256f", ATTRS{idProduct}=="c621", MODE="0666"
KERNEL=="hidraw*", ATTRS{idVendor}=="256f", ATTRS{idProduct}=="c623", MODE="0666"
KERNEL=="hidraw*", ATTRS{idVendor}=="256f", ATTRS{idProduct}=="c625", MODE="0666"
KERNEL=="hidraw*", ATTRS{idVendor}=="256f", ATTRS{idProduct}=="c626", MODE="0666"
KERNEL=="hidraw*", ATTRS{idVendor}=="256f", ATTRS{idProduct}=="c627", MODE="0666"
KERNEL=="hidraw*", ATTRS{idVendor}=="256f", ATTRS{idProduct}=="c628", MODE="0666"
KERNEL=="hidraw*", ATTRS{idVendor}=="256f", ATTRS{idProduct}=="c629", MODE="0666"
KERNEL=="hidraw*", ATTRS{idVendor}=="256f", ATTRS{idProduct}=="c62b", MODE="0666"
KERNEL=="hidraw*", ATTRS{idVendor}=="256f", ATTRS{idProduct}=="c62e", MODE="0666"
KERNEL=="hidraw*", ATTRS{idVendor}=="256f", ATTRS{idProduct}=="c62f", MODE="0666"
KERNEL=="hidraw*", ATTRS{idVendor}=="256f", ATTRS{idProduct}=="c631", MODE="0666"
KERNEL=="hidraw*", ATTRS{idVendor}=="256f", ATTRS{idProduct}=="c632", MODE="0666"
KERNEL=="hidraw*", ATTRS{idVendor}=="256f", ATTRS{idProduct}=="c633", MODE="0666"
KERNEL=="hidraw*", ATTRS{idVendor}=="256f", ATTRS{idProduct}=="c635", MODE="0666"
KERNEL=="hidraw*", ATTRS{idVendor}=="256f", ATTRS{idProduct}=="c636", MODE="0666"
KERNEL=="hidraw*", ATTRS{idVendor}=="256f", ATTRS{idProduct}=="c640", MODE="0666"
KERNEL=="hidraw*", ATTRS{idVendor}=="256f", ATTRS{idProduct}=="c652", MODE="0666"
```

Restart your computer and you should now be able to use your SpaceMouse in Unreal Engine.
