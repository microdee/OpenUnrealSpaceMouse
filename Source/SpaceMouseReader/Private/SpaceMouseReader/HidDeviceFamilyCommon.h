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
	TSharedRef<FDeviceModel> MakeHidDeviceModel(FStringView const& name, uint16 vid, uint16 pid, EModelConfidence confidence, ExtraReaderArgs&&... extraReaderArgs)
	{
		return MakeShared<FDeviceModel>(confidence)
			->With<FDeviceId>()->With([&name, vid, pid](FDeviceId& id)
			{
				id.With(new FDeviceModelName(name)).With(new Hid::FHidDeviceId(vid, pid));
			})
			->With(new FCreateHidDevice([extraReaderArgs...](FDevice& device, Hid::FHidDeviceInfo const& info)
			{
				device.With<Family>()->With(new Reader(info, extraReaderArgs...));
			}));
	}
}