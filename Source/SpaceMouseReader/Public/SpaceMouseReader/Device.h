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
#include "SpaceMouseReader/DeviceOutput.h"
#include "SpaceMouseReader/MovementState.h"
#include "SpaceMouseReader/UserSettings.h"
#include "Mcro/Common.h"

namespace SpaceMouse::Reader
{
	using namespace Mcro::Common;

	class IDeviceFamily;
	class IDeviceReader;
	class FDeviceId;

	class FDevice;
	using FDevicePtr = TSharedPtr<FDevice>;
	using FDeviceRef = TSharedRef<FDevice>;
	using FDeviceWeakPtr = TWeakPtr<FDevice>;
	
	class SPACEMOUSEREADER_API FDevice : public IComposable, public TSharedFromThis<FDevice>
	{
	public:
		FDevice();
		FDevice(FDevice&&) = default;
		FDevice(const FDevice&) = delete;
		FDevice& operator = (const FDevice&) = delete;

		TValueThunk<FSmUserSettings> UserSettings;
		FDeviceOutput ProcessedData;
		FDeviceOutput NormData;
		FMovementState MovementState;
		
		TState<IErrorPtr> LastError;
		
		FDeviceId&       GetId();
		FDeviceId const& GetId() const;
		
		IDeviceReader&       GetReader();
		IDeviceReader const& GetReader() const;
		
		IDeviceFamily&       GetFamily();
		IDeviceFamily const& GetFamily() const;

		void Tick(float deltaSecs);

		bool IsValid() const;
		
	protected:
		IDeviceFamily* Family = nullptr;
		IDeviceReader* Reader = nullptr;
		FDeviceId* Id = nullptr;
	};
}