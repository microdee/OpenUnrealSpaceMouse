// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceMouseReader/MovementState.h"

namespace SpaceMouse::Reader
{
	void FMovementState::FlagMove()
	{
		bMoving = true;
	}

	void FMovementState::PreTick()
	{
		bPrevMoving = MovementTimed > 0;
		bMoving = false;
	}

	void FMovementState::Tick(float MovementTimeTolerance, float DeltaSeconds)
	{
		if(bMoving) MovementTimed = MovementTimeTolerance;
    
		bOnMovementStartedFrame = MovementTimed > 0 && !bPrevMoving;
		if(bOnMovementStartedFrame) Moving = true;
    
		MovementTimed -= DeltaSeconds;
    
		bOnMovementEndedFrame = MovementTimed <= 0 && bPrevMoving;
		if(bOnMovementEndedFrame) Moving = false;
	}

	void FMovementState::AccumulationReset()
	{
		bOnMovementStartedFrame = false;
		bOnMovementEndedFrame = false;
		bMoving = false;
	}

	void FMovementState::Accumulate(const FMovementState& Other)
	{
		bMoving |= Other.bMoving;
		bOnMovementStartedFrame |= Other.bOnMovementStartedFrame;
		bOnMovementEndedFrame |= Other.bOnMovementEndedFrame;
    
		if(bOnMovementStartedFrame) Moving = true;
		if(bOnMovementEndedFrame) Moving = false;
	}
}
