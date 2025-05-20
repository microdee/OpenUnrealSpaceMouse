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
#include "Mcro/Common.h"

namespace SpaceMouse::Reader
{
	using namespace Mcro::Common;
	
	class SPACEMOUSEREADER_API FMovementState
	{
	public:
		
		bool bOnMovementStartedFrame = false;
		bool bOnMovementEndedFrame = false;
		FBool Moving {false};

		void FlagMove();
		void PreTick();
		void Tick(float MovementTimeTolerance, float DeltaSeconds);
		void AccumulationReset();
		void Accumulate(const FMovementState& Other);
		
	private:
		bool bMoving = false;
		bool bPrevMoving = false;
		float MovementTimed = 0.0f;
	};
}
