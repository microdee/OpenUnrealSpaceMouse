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




#include "ProcessedDeviceOutput.h"

FSmProcessedDeviceOutput FSmProcessedDeviceOutput::operator+(const FSmProcessedDeviceOutput& other) const
{
    return {
        Translation + other.Translation,
        Rotation + other.Rotation,
        Buttons | other.Buttons
    };
}

FSmProcessedDeviceOutput FSmProcessedDeviceOutput::operator+(TSharedPtr<FSmProcessedDeviceOutput> other) const
{
    return {
        Translation + other->Translation,
        Rotation + other->Rotation,
        Buttons | other->Buttons
    };
}

FSmProcessedDeviceOutput&& FSmProcessedDeviceOutput::operator+=(const FSmProcessedDeviceOutput& other)
{
    Translation += other.Translation;
    Rotation += other.Rotation;
    Buttons |= other.Buttons;
    return MoveTemp(*this);
}

FSmProcessedDeviceOutput&& FSmProcessedDeviceOutput::operator+=(TSharedPtr<FSmProcessedDeviceOutput> other)
{
    Translation += other->Translation;
    Rotation += other->Rotation;
    Buttons |= other->Buttons;
    return MoveTemp(*this);
}
