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
#include "SpaceMouseReader/DeviceModel.h"
#include "Mcro/Common.h"

namespace SpaceMouse::Reader
{
	using namespace Mcro::Common;

	class SPACEMOUSEREADER_API IDeviceFamily
	{
	public:
		virtual ~IDeviceFamily() = default;
		
		TArray<Buttons::ECmd> SupportedButtons;
		virtual void ProcessRawButtons(FDeviceOutput& target) {}

		class IFactory : public TAutoModularFeature<IFactory>
		{
		public:
			virtual TArray<FDeviceModel> const& GetKnownDeviceModels() = 0;
		};
		virtual IFactory& GetFactory() = 0;
	};
}
