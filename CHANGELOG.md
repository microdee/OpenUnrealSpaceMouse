
# **2.0** (Current)

* Modernized source code refactored the entire plugin to be very modular
* License changed to MPL instead of MIT
* Devices are discovered during runtime, no need to restart the editor for connecting a device.
* SpaceMouse Enterprise has all its buttons working.
* Set default 3DxWare profiles for Unreal (and packaged applications) which no longer has mouse-wheel emulations and button bindings
  * This fixes the ancient "bug" of "camera is jumping forward/backward"
* Camera rotation speed in CameraDelta modes are dependent on the Viewport FoV

# **1.3**

* Space-mice can navigate now visual DSL graphs in the editor (Blueprints, Materials, Niagara, etc.) (PR by [litruv](https://github.com/litruv))
* Comply with viewport changes introduced in Unreal 5.6

## 1.3.1 (Fab)

* Fix plugin platform filtering

# **1.2**

* Completely rewrite the SpaceMouseReader module to support more data sources and just have better code quality
* Participate properly in Unreal's Input Device system (have bindable `FKey`s and axes)
* Trigger Editor commands directly
* Support Orthographic viewports

## 1.2.4

* Release to Marketplace, only compliance changes are made

## 1.2.3-β
* *RUNTIME*
  * SpaceMouse buttons are now properly registered as `FKey`s and as such can be used as Blueprint input events or input events in Slate/UMG widgets
  * Separated the single static Blueprint device data getter, into multiple nodes
* *EDITOR*
  * **BREAKING CHANGE!** Configuration refactored to support new SpaceMouse `FKey`'s
    * Configuration from previous versions will not work.
  * Use the `Keyboard Shortcuts` editor preferences to bind SpaceMouse buttons to any of the pre-existing editor commands
  * Added a "universal" default button binding until the per-device system is ready.
    * User needs to trigger this configuration from the SpaceMouse preferences
  * Added support for Orthographic viewports
  * Binding the `MenuOptions` SpaceMouse button to open the SpaceMouse preferences by default.
  * **BREAKING CHANGE!** Fixed stupid spelling error for Orbiting (with one T).
    * User has to re-select orbiting camera behaviors, and reconfigure their settings

## 1.2.2-α
* *EDITOR*
  * Internal button mappings for devices are complete (copied from Blender)
  * Fixed a bug with Universal Receiver not having
  * Started MacOS support but ran into a roadblock issue

## 1.2.1-α
* *EDITOR*
  * Started support for device type detection and button configuration
  * Test release for the SpaceMouseReader rewrite

## 1.2.0-α
* *READER*
  * Rewrote the device data reader module so it can support more features, and it has higher quality of code and architecture
  * Among many other benefits it introduced Normalized device data as output.
  * Test release for the SpaceMouseReader rewrite

# **1.1** (Current) (Marketplace)

## <= 1.1.4 
[See releases](https://github.com/microdee/UE4-SpaceMouse/releases)

# **?.?** (future)

### **Goals:**
* [x] Deal with the TDxUnrealEd collision
  * This is dealt with creating xml config files in 3DxWare user settings folder.
* Use space mice in any spatial editor, not just viewports like
  * [x] Blueprint/Material/Niagara/etc graph editors
  * [ ] Sequencers/timelines
  * [ ] Curve editors

