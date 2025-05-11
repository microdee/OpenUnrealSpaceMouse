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

#include "SmButtons.h"
#include "Containers/StaticBitArray.h"

/**
 * 
 */
struct SPACEMOUSEREADER_API FSmProcessedDeviceOutput
{
    FVector Translation {0,0,0};
    FRotator Rotation {0,0,0};
    FButtonBits Buttons;

    FSmProcessedDeviceOutput operator+(const FSmProcessedDeviceOutput& other) const;
    FSmProcessedDeviceOutput operator+(TSharedPtr<FSmProcessedDeviceOutput> other) const;
    FSmProcessedDeviceOutput&& operator+=(const FSmProcessedDeviceOutput& other);
    FSmProcessedDeviceOutput&& operator+=(TSharedPtr<FSmProcessedDeviceOutput> other);
};
