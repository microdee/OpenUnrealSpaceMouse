/** @noop License Comment
 *  @file
 *  @copyright
 *  This Source Code is subject to the terms of the Mozilla Public License, v2.0.
 *  If a copy of the MPL was not distributed with this file You can obtain one at
 *  https://mozilla.org/MPL/2.0/
 *  
 *  @author David Mórász
 *  @date 2025
 */

#include "SpaceMouseReader/HidDeviceModel.h"

#include "SpaceMouseReader/Device.h"
#include "SpaceMouseReader/HidapiLayer.h"

namespace SpaceMouse::Reader
{
	FCreateHidDevice::FCreateHidDevice(SetupFunc&& setup)
		: Setup(Forward<SetupFunc>(setup))
	{}

	// TODO: fix IComposable so it can provide safe parent reference mechanism without smart pointers
	TMaybe<FDevice> FCreateHidDevice::CreateHidDevice(Hid::FHidDeviceInfo const& info, FDeviceModel const& parent) const
	{
		ASSERT_CRASH(Setup);
		FDevice output;
		output.WithAnsi(Ansi::New<FDeviceId>(parent.Id)).With([&info](FDeviceId& id)
		{
			id.WithAnsi(Ansi::New<Hid::FHidDevicePath>(info.Path));
		});
		Setup(output, info);
		if (output.LastError->IsValid())
		{
			return output.LastError.Get().ToSharedRef();
		}
		return output;
	}
}
