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
class SPACEMOUSEREADER_API FUniversalReceiver : public FSmButtonCapabilities
{
public:
    FUniversalReceiver()
    {
        // TODO: detect actual device and use those buttons
    }

    virtual void TransformRawData(TSharedPtr<FSmProcessedDeviceOutput> Data) override;
};
