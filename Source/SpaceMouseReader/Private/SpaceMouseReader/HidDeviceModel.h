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

	struct SPACEMOUSEREADER_API FCreateHidDevice : IStrictComponent
	{
		using SetupFunc = TFunction<void(FDevice&, Hid::FHidDeviceInfo const&)>;
		
		FCreateHidDevice(SetupFunc&& setup);

		void OnComponentRegistered(const FDeviceModel& model);

		bool MatchesWith(Hid::FHidDeviceInfo const& info) const;
		TMaybe<FDevice> CreateHidDevice(Hid::FHidDeviceInfo const& info) const;

		FDeviceId Id;
		SetupFunc Setup;

		Hid::FHidDeviceId* HidId = nullptr;
	};
}