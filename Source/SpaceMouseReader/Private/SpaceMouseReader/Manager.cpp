// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceMouseReader/Manager.h"
#include "SpaceMouseReader/HidapiLayer.h"
#include "Logging/StructuredLog.h"

DEFINE_LOG_CATEGORY(LogSpaceMouseManager);

namespace SpaceMouse::Reader
{
	void IManager::Initialize()
	{
		LastError.OnChange([this](IErrorPtr const& error)
		{
			error->ERROR_LOG(LogSpaceMouseManager, Warning);
			if (auto hidError = error->AsExactly<Hid::FHidError>())
			{
				if (hidError->HidErrorMessage.Contains(TEXT_"not connected"))
				{
					RefreshDevices();
				}
			}
		});
		IDeviceSource::OnRegistered().Add(InferDelegate::From(SharedThis(this), [this](IDeviceSource* devSource)
		{
			DeviceSourceCache = IDeviceSource::GetAll();
			LastError.SyncPull(SharedThis(this), devSource->LastError);
		}));
	}

	void IManager::TickManager(float deltaSecs)
	{
		PreviousAccumulatedData = AccumulatedData;
		AccumulatedData = NormData = {};
		MovementState.AccumulationReset();
		
		for (IDeviceSource* source : DeviceSourceCache)
		{
			for (auto const& device : source->Devices)
			{
				AccumulatedData += device->ProcessedData;
				NormData += device->NormData;
				MovementState.Accumulate(device->MovementState);
			}
		}

		for (auto& [cmd, state] : Buttons)
			state.NormalizePrevious();

		for (uint16 button : AccumulatedData.ButtonQueue)
		{
			auto cmd = Buttons::AsCmd(button - 1);
			if (cmd == Buttons::ECmd::Noop) continue;
			if (!PreviousAccumulatedData.ButtonQueue.ArrayView().Contains(button))
				Buttons.FindOrAdd(cmd) = true;
		}
		for (uint16 button : PreviousAccumulatedData.ButtonQueue)
		{
			auto cmd = Buttons::AsCmd(button - 1);
			if (cmd == Buttons::ECmd::Noop) continue;
			if (!AccumulatedData.ButtonQueue.ArrayView().Contains(button))
				Buttons.FindOrAdd(cmd) = false;
		}
	}

	FBool const& IManager::GetButton(Buttons::ECmd cmd)
	{
		return Buttons.FindOrAdd(cmd, FBool(false));
	}

	void IManager::RefreshDevices()
	{
		for (IDeviceSource* source : DeviceSourceCache) source->RefreshDevices();
	}

	bool IManager::IsAnyDeviceAvailable() const
	{
		for (IDeviceSource* source : DeviceSourceCache)
		{
			if (!source->Devices.IsEmpty()) return true;
		}
		return false;
	}
}
