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

	void FCreateHidDevice::OnComponentRegistered(FDeviceModel const& model)
	{
		using namespace SpaceMouse::Reader::Hid;
		
		Id = model.Id;
		HidId = Id.TryGet<FHidDeviceId>();
		ASSERT_CRASH(HidId);
	}

	bool FCreateHidDevice::MatchesWith(Hid::FHidDeviceInfo const& info) const
	{
		ASSERT_CRASH(HidId);
		return HidId->VidPid.Vid == info.VendorId && HidId->VidPid.Pid == info.ProductId; 
	}

	TMaybe<FDevice> FCreateHidDevice::CreateHidDevice(Hid::FHidDeviceInfo const& info) const
	{
		// It is morally wrong to call this function before either of these are valid, so that's why we're crashing
		// here otherwise.
		ASSERT_CRASH(HidId && Setup);
		FDevice output;
		output.WithAnsi(Ansi::New<FDeviceId>(Id)).With([&info](FDeviceId& id)
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
