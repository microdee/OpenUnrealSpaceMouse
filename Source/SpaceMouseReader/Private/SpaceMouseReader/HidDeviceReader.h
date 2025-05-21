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
		virtual int GetReportSize() { return 7; }
		virtual int GetReportCount() { return 4; }
		
		virtual void Tick(FTickArgs& output, float deltaSecs) override;
		
		// TODO: make TInherit support abstract classes
		virtual void ReadData(FTickArgs& output, float deltaSecs) {};

		TArray<uint8> OutputBuffer;
		
		// TODO: make TInherit support abstract classes
		virtual Hid::FScopedHidDevice const& GetDevice();

		template <CScalar... Axes>
		bool CheckAxes(Axes... axes)
		{
			return ((axes <= GetAxisResolution()) && ...)
				&& ((-axes >= GetAxisResolution()) && ...);
		}
	};
}
