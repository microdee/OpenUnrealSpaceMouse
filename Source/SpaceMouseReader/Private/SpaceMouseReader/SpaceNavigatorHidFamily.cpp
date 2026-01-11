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

#include "SpaceMouseReader.h"
#include "SpaceMouseReader/Device.h"
#include "SpaceMouseReader/HidapiLayer.h"
#include "SpaceMouseReader/HidDeviceModel.h"
#include "SpaceMouseReader/HidDeviceFamilyCommon.h"
#include "SpaceMouseReader/SeparateReportTransRotHidReader.h"
#include "SpaceMouseReader/SingleReportTransRotHidReader.h"

namespace SpaceMouse::Reader
{
	TModuleBoundObject<FSpaceMouseReaderModule, FSpaceNavigatorHidFamily::FFactory> GSpaceNavigatorHidFamily;
	
	FSpaceNavigatorHidFamily::FSpaceNavigatorHidFamily()
	{
		using namespace SpaceMouse::Reader::Buttons;
		// Source of info: https://github.com/blender/blender/blob/594f47ecd2d5367ca936cf6fc6ec8168c2b360d0/intern/ghost/intern/GHOST_NDOFManager.cpp#L101
		SupportedButtons = {
			ECmd::MenuOptions,
			ECmd::ViewFit
		};
	}

	void FSpaceNavigatorHidFamily::FFactory::SubmitKnownDeviceModels(TArray<TSharedRef<FDeviceModel>>& models)
	{
		models.Append({
			MakeHidDeviceModel<FSpaceNavigatorHidFamily, FSeparateReportTransRotHidReader>(
				TEXTVIEW_"Space Mouse Compact", 0x256f, 0xc635,
				EModelConfidence::Tested
			),
			MakeHidDeviceModel<FSpaceNavigatorHidFamily, FSingleReportTransRotHidReader>(
				TEXTVIEW_"Space Mouse Wireless (USB cable)", 0x256f, 0xc62e,
				EModelConfidence::TestedViaFeedback,
				EButtonReportSource::ButtonBits_Report3
			),
			MakeHidDeviceModel<FSpaceNavigatorHidFamily, FSingleReportTransRotHidReader>(
				TEXTVIEW_"Space Mouse Wireless (Receiver)", 0x256f, 0xc62e,
				EModelConfidence::TestedViaFeedback,
				EButtonReportSource::ButtonBits_Report3
			),
			MakeHidDeviceModel<FSpaceNavigatorHidFamily, FSeparateReportTransRotHidReader>(
				TEXTVIEW_"Space Navigator", 0x046d, 0xc626,
				EModelConfidence::Tested
			),
			MakeHidDeviceModel<FSpaceNavigatorHidFamily, FSeparateReportTransRotHidReader>(
				TEXTVIEW_"Space Navigator for Notebooks", 0x046d, 0xc628,
				EModelConfidence::UntestedShouldWork
			),
			MakeHidDeviceModel<FSpaceNavigatorHidFamily, FSeparateReportTransRotHidReader>(
				TEXTVIEW_"Space Traveler", 0x046d, 0xc623,
				EModelConfidence::UntestedShouldWork
			),
		});
	}

	IDeviceFamily::IFactory& FSpaceNavigatorHidFamily::GetFactory()
	{
		return GSpaceNavigatorHidFamily.GetChecked();
	}

}
