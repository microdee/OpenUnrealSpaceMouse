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

#include "SpaceMouseReader/SpaceMouseProHidFamily.h"

#include "SpaceMouseReader/Device.h"
#include "SpaceMouseReader/HidapiLayer.h"
#include "SpaceMouseReader/HidDeviceModel.h"
#include "SpaceMouseReader/SeparateReportTransRotHidReader.h"
#include "SpaceMouseReader/SingleReportTransRotHidReader.h"

namespace SpaceMouse::Reader
{
	FSpaceMouseProHidFamily::FSpaceMouseProHidFamily()
	{
		using namespace SpaceMouse::Reader::Buttons;
		// Source of info: https://github.com/blender/blender/blob/594f47ecd2d5367ca936cf6fc6ec8168c2b360d0/intern/ghost/intern/GHOST_NDOFManager.cpp#L101
		SupportedButtons = {
			
			// sides
			ECmd::MenuOptions,
			ECmd::ViewFit,

			// left
			ECmd::KeyCtrl,
			ECmd::KeyAlt,
			ECmd::KeyShift,
			ECmd::KeyEsc,

			// right
			ECmd::ViewRollCW, // ?
			ECmd::ViewTop,
			ECmd::ViewRight,
			ECmd::ViewFront,
			ECmd::FilterRotate, // ?

			// front
			ECmd::KeyF1,
			ECmd::KeyF2,
			ECmd::KeyF3,
			ECmd::KeyF4,
		};
	}

	void FSpaceMouseProHidFamily::ProcessRawButtons(FDeviceOutput& target)
	{
		using namespace SpaceMouse::Reader::Buttons;
		Map3DConnexionModern(target.ButtonQueue);
	}
	
	TArray GSpaceMouseProModels {
		FDeviceModel(
			FDeviceId()
				.With(new FDeviceModelName(TEXTVIEW_"Space Mouse Pro"))
				.With(new Hid::FHidDeviceId(0x046d, 0xc62b))
		)
		.With(new FCreateHidDevice([](FDevice& device, Hid::FHidDeviceInfo const& info)
		{
			device.With(new FSpaceMouseProHidFamily())
				.With(new FSeparateReportTransRotHidReader(info));
		})),
			
		FDeviceModel(
			FDeviceId()
				.With(new FDeviceModelName(TEXTVIEW_"Space Mouse Pro Wireless (USB cable)"))
				.With(new Hid::FHidDeviceId(0x256f, 0xc631))
		)
		.With(new FCreateHidDevice([](FDevice& device, Hid::FHidDeviceInfo const& info)
		{
			device.With(new FSpaceMouseProHidFamily())
				.With(new FSingleReportTransRotHidReader(info, EButtonReportSource::ButtonBits_Report3));
		})),
			
		FDeviceModel(
			FDeviceId()
				.With(new FDeviceModelName(TEXTVIEW_"Space Mouse Pro Wireless (Receiver)"))
				.With(new Hid::FHidDeviceId(0x256f, 0xc632))
		)
		.With(new FCreateHidDevice([](FDevice& device, Hid::FHidDeviceInfo const& info)
		{
			device.With(new FSpaceMouseProHidFamily())
				.With(new FSingleReportTransRotHidReader(info, EButtonReportSource::ButtonBits_Report3));
		})),
	};
	FOnce GSpaceMouseProRegisterWithAllDevices;

	TArray<FDeviceModel> const& FSpaceMouseProHidFamily::FFactory::GetKnownDeviceModels()
	{
		if (GSpaceMouseProRegisterWithAllDevices) RegisterDeviceModels(GSpaceMouseProModels);
		return GSpaceMouseProModels;
	}
	
	FSpaceMouseProHidFamily::FFactory GSpaceMouseProFamily {};

	IDeviceFamily::IFactory& FSpaceMouseProHidFamily::GetFactory()
	{
		return GSpaceMouseProFamily;
	}

}
