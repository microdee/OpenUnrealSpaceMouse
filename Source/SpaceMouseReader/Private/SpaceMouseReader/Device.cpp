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


#include "SpaceMouseReader/Device.h"
#include "SpaceMouseReader/DeviceFamily.h"
#include "SpaceMouseReader/DeviceReader.h"
#include "SpaceMouseReader/DeviceId.h"

DECLARE_LOG_CATEGORY_CLASS(LogSpaceMouseReaderDevice, Log, Log)

namespace SpaceMouse::Reader
{
	FDevice::FDevice()
	{
		OnComponentAdded = [this](FAny& component)
		{
			if (auto deviceFamily = component.TryGet<IDeviceFamily>())
			{
				Family = deviceFamily;
				return;
			}
			if (auto reader = component.TryGet<IDeviceReader>())
			{
				Reader = reader;
				LastError.SyncPull(Reader->LastError);
				return;
			}
			if (auto id = component.TryGet<FDeviceId>())
			{
				Id = id;
				return;
			}
		};
	}

	void FDevice::Tick(float deltaSecs)
	{
		if (!Family || !Reader || !Id)
		{
			ERROR_LOG(LogSpaceMouseReaderDevice, Error, IError::Make(new FAssertion())
				->WithMessage(TEXT_"Device Family, Device Reader or Id components were missing.")
				->WithCodeContext(TEXT_"!Family || !Reader || !Id")
				->AsFatal()->BreakDebugger()
				->WithCppStackTrace()
				->Notify(LastError)
			);
			return;
		}
		Reader->Tick({ProcessedData, NormData, MovementState, UserSettings}, deltaSecs);
		Family->ProcessRawButtons(NormData);
		Family->ProcessRawButtons(ProcessedData);
	}

	bool FDevice::IsValid() const
	{
		return Family && Reader && Id;
	}
}
