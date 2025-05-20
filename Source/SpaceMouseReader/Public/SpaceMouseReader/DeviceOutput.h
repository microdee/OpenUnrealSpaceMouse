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
#include "SpaceMouseReader/Buttons.h"

namespace SpaceMouse::Reader
{
	using namespace Mcro::Concepts;
	
	struct FDeviceOutput
	{
		FVector Translation {0,0,0};
		FRotator Rotation {0,0,0};
		Buttons::FButtonQueue ButtonQueue = {.All = 0};

		FORCEINLINE FDeviceOutput operator + (FDeviceOutput const& other) const
		{
			return {
				Translation + other.Translation,
				Rotation + other.Rotation,
				ButtonQueue | other.ButtonQueue
			};
		}
		
		FORCEINLINE FDeviceOutput operator + (TSharedPtr<FDeviceOutput> const& other) const
		{
			return {
				Translation + other->Translation,
				Rotation + other->Rotation,
				ButtonQueue | other->ButtonQueue
			};
		}

		template <CMutableType Self>
		Self&& operator += (this Self&& self, FDeviceOutput const& other)
		{
			self.Translation += other.Translation;
			self.Rotation += other.Rotation;
			self.ButtonQueue |= other.ButtonQueue;
			return Forward<Self>(self);
		}
		
		template <CMutableType Self>
		Self&& operator += (this Self&& self, TSharedPtr<FDeviceOutput> const& other)
		{
			self.Translation += other->Translation;
			self.Rotation += other->Rotation;
			self.ButtonQueue |= other->ButtonQueue;
			return Forward<Self>(self);
		}
	};
}
