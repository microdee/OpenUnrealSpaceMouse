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
#include "Framework/Application/IInputProcessor.h"
#include "SpaceMouseEditor/Interactors/Interactor.h"

class FEditorViewportClient;

namespace SpaceMouse::Editor::Interactor
{
	class FViewportInteraction;

	class SPACEMOUSEEDITOR_API FViewportMouseWheelSpeedChange : public IInputProcessor
	{
	public:
		virtual void Tick(const float deltaSecs, FSlateApplication& slateApp, TSharedRef<ICursor> cursor) override {}
		virtual bool HandleMouseWheelOrGestureEvent(FSlateApplication& slateApp, const FPointerEvent& wheelEvent, const FPointerEvent* gestureEvent) override;
	};

	class SPACEMOUSEEDITOR_API FViewportInteraction
		: public IWidgetInteractionContext
		, public IFeatureImplementation
	{
	public:
		FViewportInteraction();

		TState<FEditorViewportClient*> ActiveViewportClient { nullptr };
		FBool IsFocused { false };
		virtual void Tick() override;

		void ChangeCameraSpeed(float deltaCoeff);

	private:

		// Disabled because functionality was unreliable
		// bool bVpWasOrbitCamera = false;
		bool bVpWasRealtime = false;

		TSharedPtr<FViewportMouseWheelSpeedChange> MouseWheelSpeedChange;
	};
}
