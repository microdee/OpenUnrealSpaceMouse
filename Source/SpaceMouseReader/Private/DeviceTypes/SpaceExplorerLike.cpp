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




#include "DeviceTypes/SpaceExplorerLike.h"

void FSpaceExplorerLike::TransformRawData(TSharedPtr<FSmProcessedDeviceOutput> Data)
{
    // Source of info: https://github.com/blender/blender/blob/594f47ecd2d5367ca936cf6fc6ec8168c2b360d0/intern/ghost/intern/GHOST_NDOFManager.cpp#L101
    static TArray<EV3DCmd> TargetV3DCmd = {
        EV3DCmd::KeyF1,
        EV3DCmd::KeyF2,
        EV3DCmd::ViewTop,
        EV3DCmd::ViewLeft,
        EV3DCmd::ViewRight,
        EV3DCmd::ViewFront,
        EV3DCmd::KeyEsc,
        EV3DCmd::KeyAlt,
        EV3DCmd::KeyShift,
        EV3DCmd::KeyCtrl,
        EV3DCmd::ViewFit,
        EV3DCmd::MenuOptions,
        EV3DCmd::ScalePlus,
        EV3DCmd::ScaleMinus,
        EV3DCmd::FilterRotate
    };

    Data->Buttons = MapButtons(Data->Buttons, TargetV3DCmd);
}
