// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceMouseReader/DeviceSource.h"

namespace SpaceMouse::Reader
{
	void IDeviceSource::Tick(float deltaSecs)
	{
		for (auto const& device : Devices)
			device->Tick(deltaSecs);
	}

	void IDeviceSource::SetUserSettings(TValueThunk<FSmUserSettings> const& userSettings)
	{
		for (auto const& device : Devices)
			device->UserSettings = userSettings;
	}
}
