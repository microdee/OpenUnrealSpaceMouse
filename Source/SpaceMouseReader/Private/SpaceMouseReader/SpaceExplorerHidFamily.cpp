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

#include "SpaceExplorerHidFamily.h"
#include "SpaceMouseReader/SpaceExplorerHidFamily.h"

#include "SpaceMouseReader.h"
#include "SpaceMouseReader/Device.h"
#include "SpaceMouseReader/HidapiLayer.h"
#include "SpaceMouseReader/HidDeviceModel.h"
#include "SpaceMouseReader/HidDeviceFamilyCommon.h"
#include "SpaceMouseReader/SeparateReportTransRotHidReader.h"

namespace SpaceMouse::Reader
{
	TModuleBoundObject<FSpaceMouseReaderModule, FSpaceExplorerHidFamily::FFactory> GSpaceExplorerHidFamily;
	
	FSpaceExplorerHidFamily::FSpaceExplorerHidFamily()
	{
		using namespace SpaceMouse::Reader::Buttons;
		// Source of info: https://github.com/blender/blender/blob/594f47ecd2d5367ca936cf6fc6ec8168c2b360d0/intern/ghost/intern/GHOST_NDOFManager.cpp#L101
		SupportedButtons = {
			ECmd::KeyF1,
			ECmd::KeyF2,
			ECmd::ViewTop,
			ECmd::ViewLeft,
			ECmd::ViewRight,
			ECmd::ViewFront,
			ECmd::KeyEsc,
			ECmd::KeyAlt,
			ECmd::KeyShift,
			ECmd::KeyCtrl,
			ECmd::ViewFit,
			ECmd::MenuOptions,
			ECmd::ScalePlus,
			ECmd::ScaleMinus,
			ECmd::FilterRotate,
		};
	}

	void FSpaceExplorerHidFamily::ProcessRawButtons(FDeviceOutput& target)
	{
		using namespace SpaceMouse::Reader::Buttons;
		MapButtonsFromArray(target.ButtonQueue, SupportedButtons);
	}

	void FSpaceExplorerHidFamily::FFactory::SubmitKnownDeviceModels(TArray<TSharedRef<FDeviceModel>>& models)
	{
		models.Append({
			MakeHidDeviceModel<FSpaceExplorerHidFamily, FSeparateReportTransRotHidReader>(
				TEXTVIEW_"Space Explorer", 0x046d, 0xc627,
				EModelConfidence::UntestedShouldWork
			)
		});
	}

	IDeviceFamily::IFactory& FSpaceExplorerHidFamily::GetFactory()
	{
		return GSpaceExplorerHidFamily.GetChecked();
	}
}
