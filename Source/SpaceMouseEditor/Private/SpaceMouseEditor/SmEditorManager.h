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

#include "SpaceMouseReader/Manager.h"
#include "Misc/App.h"
#include "Input/Events.h"
#include "TimerManager.h"

struct FInputActionKeyMapping;
class FEditorViewportClient;

namespace SpaceMouse::Editor
{
	using namespace SpaceMouse::Reader;
	
	class FSmViewportOverlay;
	
	class SPACEMOUSEEDITOR_API FSmEditorManager
		: public IManager
		, public FTickableGameObject
	{
	private:
		FOnce ShouldInitialize;

		bool bLearning = false;
		bool bFinishLearning = false;

		bool bWasOrbitCamera = false;
		bool bWasRealtime = false;
		float LastOrbitDistance = 0;
		FVector LastOrbitPivot = FVector::ZeroVector;
		FVector LastOrbitPivotView = FVector::ZeroVector;

		// TODO: Pray to your god that it ain't gonna dangle
		FEditorViewportClient* ActiveViewportClient = nullptr;
		FString FocusedVpType {};

		TSharedPtr<FSmViewportOverlay> OrbitingOverlay;

		// TODO: Not used anywhere?
		static FKeyEvent GetKeyEventFromKey(const FInputActionKeyMapping& mapping);
		static bool AllowPerspectiveCameraMoveEvent(FEditorViewportClient* cvp);
		FVector GetOrbitingPosDeltaOffset(FRotator rotDelta, float forwardDelta);

	protected:
		virtual FSmUserSettings GetUserSettings() override;

	public:

		static FSmEditorManager& Get();

		void BeginLearning();
		void EndLearning();
		bool IsLearning() const { return bLearning; } 

		virtual void Tick(float DeltaSecs) override;
		virtual bool IsTickableWhenPaused() const override { return true; }
		virtual bool IsTickableInEditor() const override { return true; }
		virtual TStatId GetStatId() const override { return {}; }
		
		void ManageOrbitingOverlay();
		void ManageActiveViewport();
		void TriggerCustomButtons();
		void MoveActiveViewport(FVector trans, FRotator rot);
	};
}
