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

	class SPACEMOUSEREADER_API IDeviceReader
	{
	public:
		virtual ~IDeviceReader() = default;
		
		TEventDelegate<void()> OnData;

		/**
		 *	@todo Express this as IDeviceReader / IDeviceFamily combination association
		 */
		virtual int GetAxisResolution() const { return 350; }
		static float GetCurvedFloat(FRichCurve const& curve, float ff);

		TState<IErrorPtr> LastError;

		struct FTickArgs
		{
			FDeviceOutput& ProcessedData;
			FDeviceOutput& NormData;
			FMovementState& MovementState;
			FSmUserSettings const& Settings;
		};
		void Tick(FTickArgs&& output, float deltaSecs);
	
	protected:
		virtual void Tick(FTickArgs& output, float deltaSecs);
		static void TickMovementState(FTickArgs& output, float deltaSecs);
		static void ApplyTranslation(FTickArgs& output, float fx, float fy, float fz, float deltaSecs);
		static void ApplyRotation(FTickArgs& output, float fp, float fy, float fr, float deltaSecs);
	};
}