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
	class SPACEMOUSEREADER_API FSeparateReportTransRotHidReader : public TInherit<IHidDeviceReader>
	{
	public:
		FSeparateReportTransRotHidReader(Hid::FHidDeviceInfo const& devInfo);
		virtual int GetReportSize() override { return 7; }
		virtual int GetReportCount() override { return 3; }
		virtual void ReadData(FTickArgs& output, float deltaSecs) override;
		virtual Hid::FScopedHidDevice const& GetDevice() override;
		
	private:
		Hid::FScopedHidDevice Device;
	};
}