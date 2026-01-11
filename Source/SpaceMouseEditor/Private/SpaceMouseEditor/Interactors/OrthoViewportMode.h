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

namespace SpaceMouse::Editor::Interactor
{
	class SPACEMOUSEEDITOR_API FOrthoViewportMode : public IWidgetInteractionMode
	{
	public:
		FOrthoViewportMode();

		virtual void Tick(IWidgetInteractionContext& ctx) override;
		virtual bool IsValid(IWidgetInteractionContext& ctx) override;
	};
}
