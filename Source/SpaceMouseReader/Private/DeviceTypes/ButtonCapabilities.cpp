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




#include "DeviceTypes/ButtonCapabilities.h"

FButtonBits FSmButtonCapabilities::MapModernButtons(FButtonBits HIDButtons)
{
    // Source of info: https://github.com/blender/blender/blob/594f47ecd2d5367ca936cf6fc6ec8168c2b360d0/intern/ghost/intern/GHOST_NDOFManager.cpp#L91
    static TArray<EV3DCmd> TargetV3DCmd = {
        EV3DCmd::MenuOptions,
        EV3DCmd::ViewFit,
        EV3DCmd::ViewTop,
        EV3DCmd::ViewLeft,
        EV3DCmd::ViewRight,
        EV3DCmd::ViewFront,
        EV3DCmd::ViewBottom,
        EV3DCmd::ViewBack,
        EV3DCmd::ViewRollCW,
        EV3DCmd::ViewRollCCW,
        EV3DCmd::ViewIso1,
        EV3DCmd::ViewIso2,
        EV3DCmd::KeyF1,
        EV3DCmd::KeyF2,
        EV3DCmd::KeyF3,
        EV3DCmd::KeyF4,
        EV3DCmd::KeyF5,
        EV3DCmd::KeyF6,
        EV3DCmd::KeyF7,
        EV3DCmd::KeyF8,
        EV3DCmd::KeyF9,
        EV3DCmd::KeyF10,
        EV3DCmd::KeyEsc,
        EV3DCmd::KeyAlt,
        EV3DCmd::KeyShift,
        EV3DCmd::KeyCtrl,
        EV3DCmd::FilterRotate,
        EV3DCmd::FilterPanzoom,
        EV3DCmd::FilterDominant,
        EV3DCmd::ScalePlus,
        EV3DCmd::ScaleMinus
    };
    return MapButtons(HIDButtons, TargetV3DCmd);
}

FButtonBits FSmButtonCapabilities::MapButtons(FButtonBits HIDButtons, const TArray<EV3DCmd>& ToV3DCmd)
{
    FButtonBits Output {};
    for(int i=0; i<ToV3DCmd.Num(); ++i)
    {
        if(HIDButtons[i])
            Output[FSmButton::FromCmd(ToV3DCmd[i])] = true;
    }
    return Output;
}
