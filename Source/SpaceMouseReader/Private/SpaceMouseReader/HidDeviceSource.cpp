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

#include "SpaceMouseReader.h"
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

	TArray<TSharedRef<FDevice>> const& FHidDeviceSource::GetAvailableDevices() const
	{
		return Devices;
	}

	void FHidDeviceSource::RefreshDevices()
	{
		NextCheck = 0;
		ConnectedDevices.Reset();
		RunInThread(ENamedThreads::AnyThread, SharedThis(this), [this]
		{
			uint32 devCombo = 0;
			auto result = Hid::EnumerateDevices([this, &devCombo](int32, Hid::FHidDeviceInfo const& info)
			{
				for (auto const& weakModel : GetKnownHidDevices())
					if (auto model = weakModel.Pin())
					{
						auto&& id = model->GetId().Get<Hid::FHidDeviceId>();
						if (id.VidPid.Vid == info.VendorId && id.VidPid.Pid == info.ProductId)
						{
							ConnectedDevices.Add({weakModel, info});
							devCombo ^= id.Hash;
							return;
						}
					}
			});
			RunInGameThread(SharedThis(this), [this, devCombo, result]
			{
				if (result.HasError())
					LastError = result.GetError();
				
				if (DeviceCombination.HasChangedFrom(devCombo))
				{
					ConnectAvailableDevices();
					OnDevicesChanged.Broadcast();
				}
			});
		});
	}

	void FHidDeviceSource::Tick(float deltaSecs)
	{
		if (NextCheck >= CVarCheckRate.GetValueOnAnyThread())
		{
			RefreshDevices();
		}
		NextCheck += deltaSecs;
		IDeviceSource::Tick(deltaSecs);
		RemoveFlaggedDevices();
	}

	ranges::any_view<TWeakPtr<FDeviceModel>> FHidDeviceSource::GetKnownHidDevices()
	{
		namespace rv = ranges::views;
		return GetAllKnownDeviceModels()
			| rv::filter([](TSharedRef<FDeviceModel> const& model) -> bool
			{
				return model->TryGet<FCreateHidDevice>() && model->GetId().TryGet<Hid::FHidDeviceId>();
			})
			| rv::transform([](TSharedRef<FDeviceModel> const& model)
			{
				return model.ToWeakPtr();
			})
		;
	}

	void FHidDeviceSource::ConnectAvailableDevices()
	{
		Devices.Reset(ConnectedDevices.Num());
		for (FModel const& model : ConnectedDevices)
			if (auto deviceModel = model.Model.Pin())
			{
				auto&& deviceCreator = deviceModel->Get<FCreateHidDevice>();
				if (auto result = deviceCreator.CreateHidDevice(model.Info))
				{
					Devices.Add(result.GetValue());
					result.GetValue()->LastError.OnChange(SharedThis(this), [this, info = model.Info](IErrorPtr const& error)
					{
						LastError = error;
						if (auto hidError = error->AsExactly<Hid::FHidError>())
						{
							if (hidError->HidErrorMessage.Contains(TEXT_"not connected"))
							{
								FlagDeviceRemove(UnrealConvert(info.Path));
							}
						}
					});
					LastError.SyncPull(SharedThis(this), result.GetValue()->LastError);
				}
				else
				{
					LastError = result.GetError();
				}
			}
	}

	void FHidDeviceSource::FlagDeviceRemove(FString const& path)
	{
		int removeAt = Devices.IndexOfByPredicate([&path](TSharedRef<FDevice> const& device)
		{
			auto&& devPath = device->GetId().Get<Hid::FHidDevicePath>();
			return path.Equals(devPath.Name);
		});
		RemoveAt.Enqueue(removeAt);
		ConnectedDevices.RemoveAll([&path](FModel const& model)
		{
			return UnrealConvert(model.Info.Path).Equals(path);
		});
	}

	void FHidDeviceSource::RemoveFlaggedDevices()
	{
		int removeAt = -1;
		while (RemoveAt.Dequeue(removeAt))
			Devices.RemoveAt(removeAt);
	}

	TModuleBoundObject<FSpaceMouseReaderModule, FHidDeviceSource> GHidDeviceSource {};
}
