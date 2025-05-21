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

#include "SpaceMouseReader/DeviceFamily.h"

namespace SpaceMouse::Reader
{
	TArray<FDeviceModel> const& IDeviceFamily::IFactory::GetKnownDeviceModels()
	{
		// TODO: make TInherit support abstract classes
		static TArray<FDeviceModel> dummy;
		FORCE_CRASH();
		return dummy;
	}

	IDeviceFamily::IFactory& IDeviceFamily::GetFactory()
	{
		// TODO: make TInherit support abstract classes
		static IFactory dummy;
		FORCE_CRASH();
		return dummy;
	}
}
