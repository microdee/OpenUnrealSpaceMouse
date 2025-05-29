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

#pragma once

#include "CoreMinimal.h"
#include "SpaceMouseReader/Device.h"
#include "SpaceMouseReader/HidapiLayer.h"
#include "SpaceMouseReader/HidDeviceModel.h"

namespace SpaceMouse::Reader
{
	using namespace Mcro::Common;
	
	template <typename Family, typename Reader, typename... ExtraReaderArgs>
	FDeviceModel MakeHidDeviceModel(FStringView const& name, uint16 vid, uint16 pid, EModelConfidence confidence, ExtraReaderArgs&&... extraReaderArgs)
	{
		return FDeviceModel(
			FDeviceId()
				.WithAnsi(Ansi::New<FDeviceModelName>(name))
				.WithAnsi(Ansi::New<Hid::FHidDeviceId>(vid, pid)),
			confidence
		)
		.WithAnsi(Ansi::New<FCreateHidDevice>([extraReaderArgs...](FDevice& device, Hid::FHidDeviceInfo const& info)
		{
			device.WithAnsi(Ansi::New<Family>())
				.WithAnsi(Ansi::New<Reader>(info, extraReaderArgs...));
		}));
	}
}