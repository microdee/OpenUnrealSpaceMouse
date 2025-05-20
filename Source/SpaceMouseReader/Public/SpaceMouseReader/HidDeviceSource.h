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

#include "SpaceMouseReader/DeviceSource.h"
#include "SpaceMouseReader/DeviceModel.h"
#include "SpaceMouseReader/HidapiLayer.h"

namespace SpaceMouse::Reader
{
	using namespace Mcro::Common;
	
	class SPACEMOUSEREADER_API FHidDeviceSource
		: public IDeviceSource
		, public IFeatureImplementation
	{
	public:
		FHidDeviceSource();
		virtual TArray<FDevice> const& GetAvailableDevices() const override;
		virtual void RefreshDevices() override;
		
		virtual void Tick(float deltaSecs) override;

	protected:
		struct FModel
		{
			const FDeviceModel* Model;
			Hid::FHidDeviceInfo Info;
		};
		TArray<FModel> ConnectedDevices;
		TState<uint32> DeviceCombination {0};
		float NextCheck = UE_MAX_FLT;
		
		static ranges::any_view<const FDeviceModel*> GetKnownHidDevices();
		void ConnectAvailableDevices();
	};
}