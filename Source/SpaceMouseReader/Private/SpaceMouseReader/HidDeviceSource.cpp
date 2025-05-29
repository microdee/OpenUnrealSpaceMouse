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

#include "SpaceMouseReader/HidDeviceSource.h"
#include "SpaceMouseReader/HidDeviceModel.h"

namespace SpaceMouse::Reader
{
	TAutoConsoleVariable CVarCheckRate
	{
		TEXT_"SpaceMouse.Reader.Hid.ConnectedDeviceCheckRate",
		1.6f,
		TEXT_"How often Open Unreal Space Mouse would check for connected devices"
	};
	
	FHidDeviceSource::FHidDeviceSource()
	{
		Register();
	}

	TArray<FDevice> const& FHidDeviceSource::GetAvailableDevices() const
	{
		return Devices;
	}

	void FHidDeviceSource::RefreshDevices()
	{
		ConnectedDevices.Reset();
		uint32 devCombo = 0;
		auto result = Hid::EnumerateDevices([this, &devCombo](int32, Hid::FHidDeviceInfo const& info)
		{
			for (const FDeviceModel* model : GetKnownHidDevices())
			{
				auto& id = model->Id.Get<Hid::FHidDeviceId>();
				if (id.VidPid.Vid == info.VendorId && id.VidPid.Pid == info.ProductId)
				{
					ConnectedDevices.Add({model, info});
					devCombo ^= id.Hash;
					return;
				}
			}
		});
		
		if (result.HasError())
			LastError = result.GetError();
		
		if (DeviceCombination.HasChangedFrom(devCombo))
		{
			ConnectAvailableDevices();
			OnDevicesChanged.Broadcast();
		}
	}

	void FHidDeviceSource::Tick(float deltaSecs)
	{
		if (NextCheck >= CVarCheckRate.GetValueOnAnyThread())
		{
			RefreshDevices();
			NextCheck = 0;
		}
		NextCheck += deltaSecs;
		IDeviceSource::Tick(deltaSecs);
	}

	ranges::any_view<const FDeviceModel*> FHidDeviceSource::GetKnownHidDevices()
	{
		namespace rv = ranges::views;
		return GetAllKnownDeviceModels()
			| rv::transform([](FDeviceModel const& model)
			{
				return &model;
			})
			| rv::filter([](const FDeviceModel* model) -> bool
			{
				return model->TryGet<FCreateHidDevice>() && model->Id.TryGet<Hid::FHidDeviceId>();
			});
	}

	void FHidDeviceSource::ConnectAvailableDevices()
	{
		Devices.Reset(ConnectedDevices.Num());
		for (FModel const& model : ConnectedDevices)
		{
			auto&& deviceCreator = model.Model->Get<FCreateHidDevice>();
			if (auto result = deviceCreator.CreateHidDevice(model.Info, *model.Model))
			{
				LastError.SyncPull(result.GetValue().LastError);
				Devices.Emplace(MoveTemp(result).StealValue());
			}
			else
			{
				LastError = result.GetError();
			}
		}
	}

	FHidDeviceSource GHidDeviceSource {};
}
