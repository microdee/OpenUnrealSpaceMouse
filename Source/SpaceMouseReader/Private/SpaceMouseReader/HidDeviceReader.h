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
#include "SpaceMouseReader/DeviceReader.h"

namespace SpaceMouse::Reader
{
	namespace Hid
	{
		struct FHidDeviceInfo;
		class FScopedHidDevice;
	}
	
	class SPACEMOUSEREADER_API IHidDeviceReader : public TInherit<IDeviceReader>
	{
	protected:
		virtual int GetReportSize() = 0;
		virtual int GetReportCount() = 0;
		
		virtual void Tick(FTickArgs& output, float deltaSecs) override;
		
		virtual void ReadData(FTickArgs& output, float deltaSecs) = 0;

		TArray<uint8> OutputBuffer;
		
		virtual Hid::FScopedHidDevice const& GetDevice() = 0;

		template <CScalar... Axes>
		bool CheckAxes(Axes... axes)
		{
			return ((axes <= GetAxisResolution()) && ...)
				&& ((-axes >= GetAxisResolution()) && ...);
		}
	};
}
