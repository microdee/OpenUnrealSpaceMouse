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

#include "SpaceMouseReader/DeviceModel.h"
#include "SpaceMouseReader/DeviceFamily.h"

namespace SpaceMouse::Reader
{
	TArray<FDeviceModel> GAllKnownDeviceModels {};

	FDeviceModel::FDeviceModel(FDeviceId&& deviceId, EModelConfidence confidence)
		: IComposable()
		, Id(Forward<FDeviceId>(deviceId))
		, Confidence(confidence)
	{}

	FDeviceModel::FDeviceModel(const FDeviceModel& Other)
		: IComposable(Other)
		, Id(Other.Id)
		, Confidence(Other.Confidence)
	{
	}

	FDeviceModel::FDeviceModel(FDeviceModel&& Other) noexcept
		: IComposable(Forward<FDeviceModel>(Other))
		, Id(MoveTemp(Other.Id))
		, Confidence(Other.Confidence)
	{}

	void RegisterDeviceModels(TArray<FDeviceModel> const& models)
	{
		GAllKnownDeviceModels.Append(models);
	}

	TArray<FDeviceModel> const& GetAllKnownDeviceModels()
	{
		if (GAllKnownDeviceModels.IsEmpty())
		{
			for (auto familyFactory : IDeviceFamily::IFactory::GetAll())
			{
				familyFactory->SubmitKnownDeviceModels(GAllKnownDeviceModels);
			}
		}
		return GAllKnownDeviceModels;
	}
}
