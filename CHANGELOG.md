
# **2.0** (WIP)

### **Goals:**

[ ] Modernize source code

And probably that's it. If there's a miracle and this project gets new momentum then new features will be in 2.1 or something. But this is not a promise.

# **1.3**

That didn't happen.

# **1.2** (Current) (Marketplace)

### **Goals:**
* [x] Completely rewrite the SpaceMouseReader module to support more data sources and just have better code quality
* [x] Participate properly in Unreal's Input Device system (have bindable `FKey`s and axes)
* [x] Trigger Editor commands directly
* [x] Support Orthographic viewports

## 1.2.4

* Release to Marketplace, only compliance changes are made

## 1.2.3-β
* *RUNTIME*
  * __^__ SpaceMouse buttons are now properly registered as `FKey`s and as such can be used as Blueprint input events or input events in Slate/UMG widgets
  * __^__ Separated the single static Blueprint device data getter, into multiple nodes
* *EDITOR*
  * __\+__ **BREAKING CHANGE!** Configuration refactored to support new SpaceMouse `FKey`'s
    * Configuration from previous versions will not work.
  * __\+__ Use the `Keyboard Shortcuts` editor preferences to bind SpaceMouse buttons to any of the pre-existing editor commands
  * __\+__ Added a "universal" default button binding until the per-device system is ready.
    * User needs to trigger this configuration from the SpaceMouse preferences
  * __\+__ Added support for Orthographic viewports
  * __\+__ Binding the `MenuOptions` SpaceMouse button to open the SpaceMouse preferences by default.
  * __^__ **BREAKING CHANGE!** Fixed stupid spelling error for Orbiting (with one T).
    * User has to re-select orbiting camera behaviors, and reconfigure their settings

## 1.2.2-α
* *EDITOR*
  * __^__ Internal button mappings for devices are complete (copied from Blender)
  * __^__ Fixed a bug with Universal Receiver not having
  * __\+__ Started MacOS support but ran into a roadblock issue

## 1.2.1-α
* *EDITOR*
  * __\+__ Started support for device type detection and button configuration
  * __\+__ Test release for the SpaceMouseReader rewrite

## 1.2.0-α
* *READER*
  * __^__ Rewrote the device data reader module so it can support more features, and it has higher quality of code and architecture
  * __\+__ Among many other benefits it introduced Normalized device data as output.
  * __\+__ Test release for the SpaceMouseReader rewrite

# **1.1** (Current) (Marketplace)

## <= 1.1.4 
[See releases](https://github.com/microdee/UE4-SpaceMouse/releases)

# **?.?** (future)

### **Goals:**
* [ ] Deal with the TDxUnrealEd collision
* [ ] Use space mice in any spatial editor, not just viewports like
  * Blueprint/Material/Niagara/etc graph editors
  * Sequencers/timelines
  * Curve editors
  * etc

