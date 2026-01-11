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
#include "SpaceMouseEditor/Interactors/ViewportInteraction.h"

namespace SpaceMouse::Editor
{
	class FSmViewportOverlay;

	namespace Interactor
	{
		class SPACEMOUSEEDITOR_API FPerspectiveViewportMode : public IWidgetInteractionMode
		{
		public:
			FPerspectiveViewportMode();

			virtual void Tick(IWidgetInteractionContext& ctx) override;
			virtual bool IsValid(IWidgetInteractionContext& ctx) override;

		private:
			FVector GetOrbitingPosDeltaOffset(FViewportInteraction& ctx, FRotator rotDelta, float forwardDelta);
			TSharedPtr<FSmViewportOverlay> OrbitingOverlay;

			float LastOrbitDistance = 0;
			FVector LastOrbitPivot = FVector::ZeroVector;
			FVector LastOrbitPivotView = FVector::ZeroVector;
		};
	}
}
