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
#include "Mcro/Common.h"
#include "SpaceMouseReader/Device.h"

namespace SpaceMouse::Reader
{
	using namespace Mcro::Common;
	
	class SPACEMOUSEREADER_API IDeviceSource
		: public TAutoModularFeature<IDeviceSource>
		, public FTickableGameObject
		, public TSharedFromThis<IDeviceSource>
	{
	public:
		IDeviceSource() = default;
		IDeviceSource(const IDeviceSource& Other) = delete;
		IDeviceSource& operator = (const IDeviceSource& Other) = delete;
		
		virtual void Tick(float deltaSecs) override;
		virtual TStatId GetStatId() const override { return {}; }
		virtual bool IsTickableInEditor() const override { return true; }
		virtual bool IsTickableWhenPaused() const override { return true; }

		TArray<TSharedRef<FDevice>> Devices {};
		TEventDelegate<void()> OnDevicesChanged;
		TState<IErrorPtr> LastError;
		
		virtual void RefreshDevices() {};
		virtual TArray<TSharedRef<FDevice>> const& GetAvailableDevices() const = 0;

		void SetUserSettings(TValueThunk<FSmUserSettings> const& userSettings);
	};
}