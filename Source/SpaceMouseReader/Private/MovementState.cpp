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




#include "MovementState.h"


FSmMovementState::FSmMovementState()
{
}

FSmMovementState::~FSmMovementState()
{
}

void FSmMovementState::Move()
{
    bMoving = true;
}

void FSmMovementState::PreTick()
{
    bPrevMoving = MovementTimed > 0;
    bMoving = false;
}

void FSmMovementState::Tick(float MovementTimeTolerance, float DeltaSeconds)
{
    if(bMoving) MovementTimed = MovementTimeTolerance;
    
    bOnMovementStartedFrame = MovementTimed > 0 && !bPrevMoving;
    if(bOnMovementStartedFrame) OnMovementStarted.Broadcast();
    
    MovementTimed -= DeltaSeconds;
    
    bOnMovementEndedFrame = MovementTimed <= 0 && bPrevMoving;
    if(bOnMovementEndedFrame) OnMovementEnded.Broadcast();
}

void FSmMovementState::AccumulationReset()
{
    bOnMovementStartedFrame = false;
    bOnMovementEndedFrame = false;
    bMoving = false;
}

void FSmMovementState::Accumulate(const FSmMovementState& Other)
{
    bMoving |= Other.bMoving;
    bOnMovementStartedFrame |= Other.bOnMovementStartedFrame;
    bOnMovementEndedFrame |= Other.bOnMovementEndedFrame;
    
    if(bOnMovementStartedFrame) OnMovementStarted.Broadcast();
    if(bOnMovementEndedFrame) OnMovementEnded.Broadcast();
}

void FSmMovementState::Accumulate(TSharedPtr<FSmMovementState> Other)
{
    Accumulate(*Other.Get());
}
