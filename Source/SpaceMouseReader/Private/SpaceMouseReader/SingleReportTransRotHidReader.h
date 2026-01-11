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
#include "SpaceMouseReader/HidDeviceReader.h"
#include "SpaceMouseReader/HidapiLayer.h"

namespace SpaceMouse::Reader
{
	enum class EButtonReportSource
	{
		ButtonBits_Report3,
		ButtonQueue_Report28
	};
	
	struct SPACEMOUSEREADER_API FTransRotReport
	{
		int16 TransX;
		int16 TransY;
		int16 TransZ;
		int16 RotX;
		int16 RotY;
		int16 RotZ;
	};
	
	class SPACEMOUSEREADER_API FSingleReportTransRotHidReader : public TInherit<IHidDeviceReader>
	{
	public:
		FSingleReportTransRotHidReader(Hid::FHidDeviceInfo const& devInfo, EButtonReportSource buttonSource);
		virtual int GetReportSize() override { return 13; }
		virtual int GetReportCount() override { return 4; }
		virtual void ReadData(FTickArgs& output, float deltaSecs) override;
		virtual Hid::FScopedHidDevice const& GetDevice() override;
		
	private:
		Hid::FScopedHidDevice Device;
		EButtonReportSource ButtonSource;
	};
}