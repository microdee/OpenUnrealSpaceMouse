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

	void FCreateHidDevice::OnCreatedAt(FDeviceModel& parent)
	{
		Parent = WeakSelf(&parent);
	}

	// TODO: fix IComposable so it can provide safe parent reference mechanism without smart pointers
	TMaybe<TSharedRef<FDevice>> FCreateHidDevice::CreateHidDevice(Hid::FHidDeviceInfo const& info) const
	{
		ASSERT_CRASH(Setup);
		if (auto parent = Parent.Pin())
		{
			auto output = MakeShared<FDevice>()
				->With(new FDeviceId(parent->GetId()))->With([&info](FDeviceId& id)
				{
					id.With(new Hid::FHidDevicePath(info.Path));
				});
			Setup(output.Get(), info);
			if (output->LastError.Get().IsValid())
			{
				return output->LastError.Get().ToSharedRef();
			}
			return output;
		}
		FORCE_CRASH(->WithMessage(TEXT_"Parent of FCreateHidDevice was invalid"));
		UNAVAILABLE();
	}
}
