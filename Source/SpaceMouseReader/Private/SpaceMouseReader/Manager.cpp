// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceMouseReader/Manager.h"
#include "Logging/StructuredLog.h"

DEFINE_LOG_CATEGORY(LogSpaceMouseManager);

namespace SpaceMouse::Reader
{
	IManager::IManager()
	{
		IDeviceSource::OnRegistered().Add(InferDelegate::From(LifespanGuard, [this](IDeviceSource* devSource)
		{
			UE_LOGFMT(LogSpaceMouseManager, Display, "Registering with manager");
			DeviceSourceCache = IDeviceSource::GetAll();
			LastError.SyncPull(devSource->LastError);
		}));
	}

	void IManager::TickManager(float deltaSecs)
	{
		PreviousAccumulatedData = AccumulatedData;
		AccumulatedData = NormData = {};
		MovementState.AccumulationReset();
		
		for (IDeviceSource* source : DeviceSourceCache)
		{
			for (FDevice& device : source->Devices)
			{
				AccumulatedData += device.ProcessedData;
				NormData += device.NormData;
				MovementState.Accumulate(device.MovementState);
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

	bool IManager::IsAnyDeviceAvailable() const
	{
		for (IDeviceSource* source : DeviceSourceCache)
		{
			if (!source->Devices.IsEmpty()) return true;
		}
		return false;
	}
}
