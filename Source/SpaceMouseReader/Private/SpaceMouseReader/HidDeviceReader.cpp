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


#include "SpaceMouseReader/HidDeviceReader.h"
#include "SpaceMouseReader/HidapiLayer.h"

namespace SpaceMouse::Reader
{
	TAutoConsoleVariable CVarMaxRead
	{
		TEXT_"SpaceMouse.Reader.Hid.MaxReadCountPerLoop",
		1024,
		TEXT_"The maximum number of reads processed from HID input queue in one go."
	};
	
	void IHidDeviceReader::Tick(FTickArgs& output, float deltaSecs)
	{
		IDeviceReader::Tick(output, deltaSecs);
		int32 size = GetReportSize() * GetReportCount();
		auto&& device = GetDevice();
		
		if (OutputBuffer.IsEmpty())
			OutputBuffer.SetNumUninitialized(size);

		int ctr = 0;

		bool Received = false;

		for (;;)
		{
			auto readResult = device.Read(OutputBuffer);
			if (readResult.HasError())
			{
				LastError = readResult.GetError();
				break;
			}
			if (readResult.GetValue() > 0 && ctr < CVarMaxRead.GetValueOnAnyThread())
			{
				Received = true;
				ReadData(output, deltaSecs);
				ctr++;
			}
			else break;
		}
		
		if(Received) OnData.Broadcast();

		TickMovementState(output, deltaSecs);
	}
}
