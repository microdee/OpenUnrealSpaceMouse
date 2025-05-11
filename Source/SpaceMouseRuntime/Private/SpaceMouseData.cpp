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



#include "SpaceMouseData.h"

#include "SmRuntimeManager.h"
#include "SmInputDevice.h"
#include "Engine/World.h"

bool USpaceMouseData::bFrameRequested = false;

FTimerDelegate USpaceMouseData::OnTickDel;

void USpaceMouseData::GetSpaceMouseDeltaAxes(FVector& DeltaTranslation, FRotator& DeltaRotation)
{
    DeltaTranslation = FSmInputDevice::Manager->GetTranslation();
    DeltaRotation = FSmInputDevice::Manager->GetRotation();
}

void USpaceMouseData::GetSpaceMouseAxes(FVector& NormalizedTranslation,
    FRotator& NormalizedRotation)
{
    
    NormalizedTranslation = FSmInputDevice::Manager->GetNormalizedTranslation();
    NormalizedRotation = FSmInputDevice::Manager->GetNormalizedRotation();
}

bool USpaceMouseData::GetSpaceMouseButtonState(int Id)
{
    return FSmInputDevice::Manager->GetButtons()[Id];
}

bool USpaceMouseData::GetSpaceMouseButtonDown(int Id)
{
    return FSmInputDevice::Manager->ButtonDownFrame(FSmButton::FromID(Id));
}

bool USpaceMouseData::GetSpaceMouseButtonUp(int Id)
{
    return FSmInputDevice::Manager->ButtonUpFrame(FSmButton::FromID(Id));
}
