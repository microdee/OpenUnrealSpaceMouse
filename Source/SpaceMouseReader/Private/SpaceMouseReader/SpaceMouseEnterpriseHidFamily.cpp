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

#include "SpaceMouseReader/SpaceMouseEnterpriseHidFamily.h"

#include "SpaceMouseReader/Device.h"
#include "SpaceMouseReader/HidapiLayer.h"
#include "SpaceMouseReader/HidDeviceModel.h"
#include "SpaceMouseReader/SingleReportTransRotHidReader.h"

namespace SpaceMouse::Reader
{
	FSpaceMouseEnterpriseHidFamily::FSpaceMouseEnterpriseHidFamily()
	{
		using namespace SpaceMouse::Reader::Buttons;
		// Source of info: https://github.com/blender/blender/blob/594f47ecd2d5367ca936cf6fc6ec8168c2b360d0/intern/ghost/intern/GHOST_NDOFManager.cpp#L101
		SupportedButtons = {
			// sides
			ECmd::MenuOptions,
			ECmd::ViewFit,
			ECmd::KeySpace,
			ECmd::ViewIso1,

			// left
			ECmd::KeyCtrl,
			ECmd::KeyAlt,
			ECmd::KeyShift,
			ECmd::KeyTab,
			ECmd::KeyEsc,
			ECmd::KeyEnter,

			// right
			ECmd::ViewRollCW,
			ECmd::ViewTop,
			ECmd::ViewRight,
			ECmd::ViewFront,
			ECmd::FilterRotate,

			// right upper
			ECmd::View1,
			ECmd::View2,
			ECmd::View3,
			ECmd::KeyDelete,

			// front
			ECmd::KeyF1,
			ECmd::KeyF2,
			ECmd::KeyF3,
			ECmd::KeyF4,
			ECmd::KeyF5,
			ECmd::KeyF6,
			ECmd::KeyF7,
			ECmd::KeyF8,
			ECmd::KeyF9,
			ECmd::KeyF10,
			ECmd::KeyF11,
			ECmd::KeyF12,
		};
	}

	void FSpaceMouseEnterpriseHidFamily::ProcessRawButtons(FDeviceOutput& target)
	{
		using namespace SpaceMouse::Reader::Buttons;
		Map3DConnexionModern(target.ButtonQueue);
	}
	
	TArray GSpaceMouseEnterpriseModels {
		FDeviceModel(
			FDeviceId()
				.With(new FDeviceModelName(TEXTVIEW_"Space Mouse Enterprise"))
				.With(new Hid::FHidDeviceId(0x256f, 0xc633))
		)
		.With(new FCreateHidDevice([](FDevice& device, Hid::FHidDeviceInfo const& info)
		{
			device.With(new FSpaceMouseEnterpriseHidFamily())
				.With(new FSingleReportTransRotHidReader(info, EButtonReportSource::ButtonQueue_Report28));
		})),
			
		// TODO: figure out a way to detect device through universal receiver
		FDeviceModel(
			FDeviceId()
				.With(new FDeviceModelName(TEXTVIEW_"Universal Receiver"))
				.With(new Hid::FHidDeviceId(0x256f, 0xc652)),
			EModelConfidence::UntestedShouldWork
		)
		.With(new FCreateHidDevice([](FDevice& device, Hid::FHidDeviceInfo const& info)
		{
			device.With(new FSpaceMouseEnterpriseHidFamily())
				.With(new FSingleReportTransRotHidReader(info, EButtonReportSource::ButtonQueue_Report28));
		}))
	};
	FOnce GSpaceMouseEnterpriseRegisterWithAllDevices;

	TArray<FDeviceModel> const& FSpaceMouseEnterpriseHidFamily::FFactory::GetKnownDeviceModels()
	{
		if (GSpaceMouseEnterpriseRegisterWithAllDevices) RegisterDeviceModels(GSpaceMouseEnterpriseModels);
		return GSpaceMouseEnterpriseModels;
	}
	
	FSpaceMouseEnterpriseHidFamily::FFactory GSpaceMouseEnterpriseFamily {};

	IDeviceFamily::IFactory& FSpaceMouseEnterpriseHidFamily::GetFactory()
	{
		return GSpaceMouseEnterpriseFamily;
	}

}
