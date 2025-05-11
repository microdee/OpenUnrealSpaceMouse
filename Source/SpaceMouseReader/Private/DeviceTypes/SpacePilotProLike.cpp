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




#include "DeviceTypes/SpacePilotProLike.h"

void FSpacePilotProLike::TransformRawData(TSharedPtr<FSmProcessedDeviceOutput> Data)
{
    Data->Buttons = MapModernButtons(Data->Buttons);
}
