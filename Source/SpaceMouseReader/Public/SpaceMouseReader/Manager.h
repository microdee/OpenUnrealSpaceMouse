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
#include "SpaceMouseReader/DeviceOutput.h"
#include "SpaceMouseReader/DeviceSource.h"
#include "SpaceMouseReader/MovementState.h"
#include "SpaceMouseReader/UserSettings.h"

DECLARE_LOG_CATEGORY_EXTERN(LogSpaceMouseManager, Log, Log)

namespace SpaceMouse::Reader
{
	using namespace Mcro::Common;
	
	class SPACEMOUSEREADER_API IManager : public TSharedFromThis<IManager>
	{
	public:
		virtual ~IManager() = default;

	protected:
		TArray<IDeviceSource*> DeviceSourceCache;
		FDeviceOutput PreviousAccumulatedData;
		FDeviceOutput AccumulatedData;
		FDeviceOutput NormData;
		TState<IErrorPtr> LastError;
		virtual FSmUserSettings GetUserSettings() = 0;
		
	public:
		void Initialize();
		
		TMap<Buttons::ECmd, FBool> Buttons;
		FMovementState MovementState;
		
		virtual void TickManager(float deltaSecs);
		
		FVector FORCEINLINE GetTranslation() const { return AccumulatedData.Translation; }
		FRotator FORCEINLINE GetRotation() const { return AccumulatedData.Rotation; }
		FVector FORCEINLINE GetNormalizedTranslation() const { return NormData.Translation; }
		FRotator FORCEINLINE GetNormalizedRotation() const { return NormData.Rotation; }
		FBool const& GetButton(Buttons::ECmd cmd);
		
		void RefreshDevices();

		bool IsAnyDeviceAvailable() const;
	};
}