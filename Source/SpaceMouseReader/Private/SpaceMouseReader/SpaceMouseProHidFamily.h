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
#include "SpaceMouseReader/DeviceFamily.h"

namespace SpaceMouse::Reader
{
	class SPACEMOUSEREADER_API FSpaceMouseProHidFamily : public TInherit<IDeviceFamily>
	{
	public:
		FSpaceMouseProHidFamily();

		virtual void ProcessRawButtons(FDeviceOutput& target) override;

		class FFactory : public IFactory, public IFeatureImplementation
		{
		public:
			FFactory() { Register(); }
			virtual void SubmitKnownDeviceModels(TArray<TSharedRef<FDeviceModel>>& models) override;
		};
		virtual IFactory& GetFactory() override;
	};
}
