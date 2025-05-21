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
#include "SpaceMouseReader/DeviceId.h"
#include "Mcro/Common.h"

namespace SpaceMouse::Reader
{
	using namespace Mcro::Common;
	
	enum class EModelConfidence : uint8
	{
		Tested,
		TestedViaFeedback,
		UntestedShouldWork,
		Unknown,
		Unsupported
	};

	struct SPACEMOUSEREADER_API FDeviceModel : IComposable
	{
		FDeviceModel(FDeviceId&& deviceId, EModelConfidence confidence = EModelConfidence::Tested);

		FDeviceId Id;
		EModelConfidence Confidence;
	};
	
	SPACEMOUSEREADER_API void RegisterDeviceModels(TArray<FDeviceModel> const& models);
	SPACEMOUSEREADER_API TArray<FDeviceModel> const& GetAllKnownDeviceModels();
}