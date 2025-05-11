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

#include "Mac/TDxWareButtonCapabilities.h"

void FTDxWareButtonCapabilities::TransformRawData(TSharedPtr<FProcessedDeviceOutput> Data)
{
    // TODO: assumption, so far all universal receiver compatible device have the same HID protocol
    // TODO: detect device types through the universal receiver
    Data->Buttons = MapModernButtons(Data->Buttons);
}
