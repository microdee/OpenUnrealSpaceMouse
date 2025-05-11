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

#include "ButtonCapabilities.h"

/**
 * 
 */
class SPACEMOUSEREADER_API FSpaceMouseEnterpriseLike : public FSmButtonCapabilities
{
public:
    FSpaceMouseEnterpriseLike()
    {
        // sides
        SupportedButtons.Add(EV3DCmd::MenuOptions);
        SupportedButtons.Add(EV3DCmd::ViewFit);
        SupportedButtons.Add(EV3DCmd::KeySpace);
        SupportedButtons.Add(EV3DCmd::ViewIso1);
        SupportedButtons.Add(EV3DCmd::ViewIso2); // blue ?

        // left
        SupportedButtons.Add(EV3DCmd::KeyCtrl);
        SupportedButtons.Add(EV3DCmd::KeyAlt);
        SupportedButtons.Add(EV3DCmd::KeyShift);
        SupportedButtons.Add(EV3DCmd::KeyTab); // OOR ?
        SupportedButtons.Add(EV3DCmd::KeyEsc);
        SupportedButtons.Add(EV3DCmd::KeyEnter); // OOR ?

        // right
        SupportedButtons.Add(EV3DCmd::ViewRollCW); // ?
        SupportedButtons.Add(EV3DCmd::ViewRollCCW); // ? // blue ?
        SupportedButtons.Add(EV3DCmd::ViewTop);
        SupportedButtons.Add(EV3DCmd::ViewBottom); // blue ?
        SupportedButtons.Add(EV3DCmd::ViewRight);
        SupportedButtons.Add(EV3DCmd::ViewLeft); // blue ?
        SupportedButtons.Add(EV3DCmd::ViewFront);
        SupportedButtons.Add(EV3DCmd::ViewBack); // blue ?
        SupportedButtons.Add(EV3DCmd::FilterRotate); // ?

        // right upper // TODO: Out of currently known HID protocol range
        SupportedButtons.Add(EV3DCmd::View1); // ?
        SupportedButtons.Add(EV3DCmd::View2); // ?
        SupportedButtons.Add(EV3DCmd::View3); // ?
        SupportedButtons.Add(EV3DCmd::SaveView1); // ? // blue ?
        SupportedButtons.Add(EV3DCmd::SaveView2); // ? // blue ?
        SupportedButtons.Add(EV3DCmd::SaveView3); // ? // blue ?
        SupportedButtons.Add(EV3DCmd::KeyDelete); // OOR ?

        // front
        SupportedButtons.Add(EV3DCmd::KeyF1);
        SupportedButtons.Add(EV3DCmd::KeyF2);
        SupportedButtons.Add(EV3DCmd::KeyF3);
        SupportedButtons.Add(EV3DCmd::KeyF4);
        SupportedButtons.Add(EV3DCmd::KeyF5);
        SupportedButtons.Add(EV3DCmd::KeyF6);
        SupportedButtons.Add(EV3DCmd::KeyF7);
        SupportedButtons.Add(EV3DCmd::KeyF8);
        SupportedButtons.Add(EV3DCmd::KeyF9);
        SupportedButtons.Add(EV3DCmd::KeyF10);
        SupportedButtons.Add(EV3DCmd::KeyF11);
        SupportedButtons.Add(EV3DCmd::KeyF12);
    }
    
    virtual void TransformRawData(TSharedPtr<FSmProcessedDeviceOutput> Data) override;
};
