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
 * Without PRO!
 */
class SPACEMOUSEREADER_API FSpacePilotLike : public FSmButtonCapabilities
{
public:
    FSpacePilotLike()
    {
        // sides
        SupportedButtons.Add(EV3DCmd::MenuOptions);
        SupportedButtons.Add(EV3DCmd::ViewFit);

        // left
        SupportedButtons.Add(EV3DCmd::KeyF1);
        SupportedButtons.Add(EV3DCmd::KeyF2);
        SupportedButtons.Add(EV3DCmd::KeyF3);
        SupportedButtons.Add(EV3DCmd::KeyF4);
        SupportedButtons.Add(EV3DCmd::KeyF5);
        SupportedButtons.Add(EV3DCmd::KeyF6); // blue ?
        SupportedButtons.Add(EV3DCmd::KeyF7); // blue ?
        SupportedButtons.Add(EV3DCmd::KeyF8); // blue ?
        SupportedButtons.Add(EV3DCmd::KeyF9); // blue ?
        SupportedButtons.Add(EV3DCmd::KeyF10); // blue ?

        // left upper
        SupportedButtons.Add(EV3DCmd::KeyEsc);
        SupportedButtons.Add(EV3DCmd::KeyCtrl);
        SupportedButtons.Add(EV3DCmd::KeyAlt);
        SupportedButtons.Add(EV3DCmd::KeyShift);

        // right
        SupportedButtons.Add(EV3DCmd::ViewRollCW); // ?
        SupportedButtons.Add(EV3DCmd::ViewRollCCW); // ? // blue ?
        SupportedButtons.Add(EV3DCmd::ViewTop);
        SupportedButtons.Add(EV3DCmd::ViewBottom); // blue ?
        SupportedButtons.Add(EV3DCmd::ViewRight);
        SupportedButtons.Add(EV3DCmd::ViewLeft); // blue ?
        SupportedButtons.Add(EV3DCmd::ViewFront);
        SupportedButtons.Add(EV3DCmd::ViewBack); // blue ?
        SupportedButtons.Add(EV3DCmd::ViewIso1);
        SupportedButtons.Add(EV3DCmd::ViewIso2); // blue ?

        // right upper        
        SupportedButtons.Add(EV3DCmd::FilterPanzoom); // ?
        SupportedButtons.Add(EV3DCmd::FilterDominant); // ?
        SupportedButtons.Add(EV3DCmd::FilterRotate); // ?
        SupportedButtons.Add(EV3DCmd::ScalePlus); // ?
        SupportedButtons.Add(EV3DCmd::ScaleMinus); // ?

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
