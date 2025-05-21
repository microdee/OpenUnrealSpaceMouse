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

#include "SpaceMouseReader/SpaceNavigatorHidFamily.h"

#include "SpaceMouseReader/Device.h"
#include "SpaceMouseReader/HidapiLayer.h"
#include "SpaceMouseReader/HidDeviceModel.h"
#include "SpaceMouseReader/SeparateReportTransRotHidReader.h"
#include "SpaceMouseReader/SingleReportTransRotHidReader.h"

namespace SpaceMouse::Reader
{
	FSpaceNavigatorHidFamily::FSpaceNavigatorHidFamily()
	{
		using namespace SpaceMouse::Reader::Buttons;
		// Source of info: https://github.com/blender/blender/blob/594f47ecd2d5367ca936cf6fc6ec8168c2b360d0/intern/ghost/intern/GHOST_NDOFManager.cpp#L101
		SupportedButtons = {
			ECmd::MenuOptions,
			ECmd::ViewFit
		};
	}

	TArray GSpaceNavigatorModels {
		FDeviceModel(
			FDeviceId()
				.With(new FDeviceModelName(TEXTVIEW_"Space Mouse Compact"))
				.With(new Hid::FHidDeviceId(0x256f, 0xc635))
		)
		.With(new FCreateHidDevice([](FDevice& device, Hid::FHidDeviceInfo const& info)
		{
			device.With(new FSpaceNavigatorHidFamily())
				.With(new FSeparateReportTransRotHidReader(info));
		})),
		
		FDeviceModel(
			FDeviceId()
				.With(new FDeviceModelName(TEXTVIEW_"Space Mouse Wireless (USB cable)"))
				.With(new Hid::FHidDeviceId(0x256f, 0xc62e))
		)
		.With(new FCreateHidDevice([](FDevice& device, Hid::FHidDeviceInfo const& info)
		{
			device.With(new FSpaceNavigatorHidFamily())
				.With(new FSingleReportTransRotHidReader(info, EButtonReportSource::ButtonBits_Report3));
		})),
		
		FDeviceModel(
			FDeviceId()
				.With(new FDeviceModelName(TEXTVIEW_"Space Mouse Wireless (Receiver)"))
				.With(new Hid::FHidDeviceId(0x256f, 0xc62f))
		)
		.With(new FCreateHidDevice([](FDevice& device, Hid::FHidDeviceInfo const& info)
		{
			device.With(new FSpaceNavigatorHidFamily())
				.With(new FSingleReportTransRotHidReader(info, EButtonReportSource::ButtonBits_Report3));
		})),
		
		FDeviceModel(
			FDeviceId()
				.With(new FDeviceModelName(TEXTVIEW_"Space Navigator"))
				.With(new Hid::FHidDeviceId(0x046d, 0xc626))
		)
		.With(new FCreateHidDevice([](FDevice& device, Hid::FHidDeviceInfo const& info)
		{
			device.With(new FSpaceNavigatorHidFamily())
				.With(new FSeparateReportTransRotHidReader(info));
		})),
		
		FDeviceModel(
			FDeviceId()
				.With(new FDeviceModelName(TEXTVIEW_"Space Navigator for Notebooks"))
				.With(new Hid::FHidDeviceId(0x046d, 0xc628)),
			EModelConfidence::UntestedShouldWork
		)
		.With(new FCreateHidDevice([](FDevice& device, Hid::FHidDeviceInfo const& info)
		{
			device.With(new FSpaceNavigatorHidFamily())
				.With(new FSeparateReportTransRotHidReader(info));
		})),
		
		FDeviceModel(
			FDeviceId()
				.With(new FDeviceModelName(TEXTVIEW_"Space Traveler"))
				.With(new Hid::FHidDeviceId(0x046d, 0xc623)),
			EModelConfidence::UntestedShouldWork
		)
		.With(new FCreateHidDevice([](FDevice& device, Hid::FHidDeviceInfo const& info)
		{
			device.With(new FSpaceNavigatorHidFamily())
				.With(new FSeparateReportTransRotHidReader(info));
		})),
	};
	FOnce GSpaceNavigatorRegisterWithAllDevices;

	TArray<FDeviceModel> const& FSpaceNavigatorHidFamily::FFactory::GetKnownDeviceModels()
	{
		if (GSpaceNavigatorRegisterWithAllDevices) RegisterDeviceModels(GSpaceNavigatorModels);
		return GSpaceNavigatorModels;
	}
	
	FSpaceNavigatorHidFamily::FFactory GSpaceNavigatorFamily {};

	IDeviceFamily::IFactory& FSpaceNavigatorHidFamily::GetFactory()
	{
		return GSpaceNavigatorFamily;
	}

}
