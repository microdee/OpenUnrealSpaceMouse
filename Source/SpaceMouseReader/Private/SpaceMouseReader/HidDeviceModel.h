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
#include "SpaceMouseReader/DeviceModel.h"
#include "Mcro/Common.h"

namespace SpaceMouse::Reader
{
	using namespace Mcro::Common;
	
	class FDevice;

	namespace Hid
	{
		struct FHidDeviceInfo;
		class FHidDeviceId;
	}

	struct SPACEMOUSEREADER_API FCreateHidDevice
	{
		using SetupFunc = TFunction<void(FDevice&, Hid::FHidDeviceInfo const&)>;
		
		FCreateHidDevice(SetupFunc&& setup);

		// TODO: fix IComposable so it can provide safe parent reference mechanism without smart pointers
		TMaybe<FDevice> CreateHidDevice(Hid::FHidDeviceInfo const& info, FDeviceModel const& parent) const;
		SetupFunc Setup;
	};
}