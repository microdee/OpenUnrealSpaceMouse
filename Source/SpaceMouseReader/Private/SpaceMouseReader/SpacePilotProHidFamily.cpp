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

#include "SpaceMouseReader/SpacePilotProHidFamily.h"

#include "SpaceMouseReader.h"
#include "SpaceMouseReader/Device.h"
#include "SpaceMouseReader/HidapiLayer.h"
#include "SpaceMouseReader/HidDeviceModel.h"
#include "SpaceMouseReader/HidDeviceFamilyCommon.h"
#include "SpaceMouseReader/SeparateReportTransRotHidReader.h"

namespace SpaceMouse::Reader
{
	TModuleBoundObject<FSpaceMouseReaderModule, FSpacePilotProHidFamily::FFactory> GSpacePilotProHidFamily;
	
	FSpacePilotProHidFamily::FSpacePilotProHidFamily()
	{
		using namespace SpaceMouse::Reader::Buttons;
		// Source of info: https://github.com/blender/blender/blob/594f47ecd2d5367ca936cf6fc6ec8168c2b360d0/intern/ghost/intern/GHOST_NDOFManager.cpp#L101
		SupportedButtons = {
			
			// sides
			ECmd::MenuOptions,
			ECmd::ViewFit,
			
			// left
			ECmd::KeyF1,
			ECmd::KeyF2,
			ECmd::KeyF3,
			ECmd::KeyF4,
			ECmd::KeyF5,

			// left upper
			ECmd::KeyEsc,
			ECmd::KeyCtrl,
			ECmd::KeyAlt,
			ECmd::KeyShift,

			// right
			ECmd::ViewRollCW,
			ECmd::ViewTop,
			ECmd::ViewRight,
			ECmd::ViewFront,
			ECmd::ViewIso1,

			// right upper
			ECmd::FilterPanzoom,
			ECmd::FilterDominant,
			ECmd::FilterRotate,
			ECmd::ScalePlus,
			ECmd::ScaleMinus,
			
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

	void FSpacePilotProHidFamily::ProcessRawButtons(FDeviceOutput& target)
	{
		using namespace SpaceMouse::Reader::Buttons;
		Map3DConnexionModern(target.ButtonQueue);
	}

	void FSpacePilotProHidFamily::FFactory::SubmitKnownDeviceModels(TArray<TSharedRef<FDeviceModel>>& models)
	{
		models.Append({
			MakeHidDeviceModel<FSpacePilotProHidFamily, FSeparateReportTransRotHidReader>(
				TEXTVIEW_"Space Pilot Pro", 0x046d, 0xc629,
				EModelConfidence::TestedViaFeedback
			)
		});
	}
	
	IDeviceFamily::IFactory& FSpacePilotProHidFamily::GetFactory()
	{
		return GSpacePilotProHidFamily.GetChecked();
	}

}
