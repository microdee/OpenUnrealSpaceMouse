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

DECLARE_MULTICAST_DELEGATE(FSmMovementEvent)

/**
 * Class storing movement state of a SpaceMouse device
 */
class SPACEMOUSEREADER_API FSmMovementState
{
public:
    FSmMovementState();
    ~FSmMovementState();

    FSmMovementEvent OnMovementStarted;
    FSmMovementEvent OnMovementEnded;
    
    bool bOnMovementStartedFrame = false;
    bool bOnMovementEndedFrame = false;
    bool bMoving = false;

    void Move();
    void PreTick();
    void Tick(float MovementTimeTolerance, float DeltaSeconds);
    void AccumulationReset();
    void Accumulate(const FSmMovementState& Other);
    void Accumulate(TSharedPtr<FSmMovementState> Other);
    
private:
    bool bPrevMoving = false;
    float MovementTimed = 0.0f;
    
};
