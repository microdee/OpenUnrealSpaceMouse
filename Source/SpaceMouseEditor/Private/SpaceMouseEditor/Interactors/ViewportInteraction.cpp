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

#include "ViewportInteraction.h"

#include "OrthoViewportMode.h"
#include "PerspectiveViewportMode.h"
#include "SEditorViewport.h"
#include "SpaceMouseEditor.h"
#include "HAL/PlatformApplicationMisc.h"
#include "SpaceMouseEditor/SmEditorManager.h"
#include "SpaceMouseEditor/SpaceMouseConfig.h"
#include "SpaceMouseRuntime/RuntimeManager.h"
#include "SpaceMouseRuntime/SmInputDevice.h"

namespace SpaceMouse::Editor::Interactor
{
	TModuleBoundObject<FSpaceMouseEditorModule, FViewportInteraction> GViewportInteraction {};

	FViewportInteraction::FViewportInteraction()
	{
		RegisterContext();
	}

	void FViewportInteraction::Tick()
	{
		namespace rv = ranges::views;
		using namespace SpaceMouse::Runtime;

		if (InteractionStack.IsEmpty())
		{
			InteractionStack.Push(MakeShared<FPerspectiveViewportMode>());
			InteractionStack.Push(MakeShared<FOrthoViewportMode>());
		}

		auto&& vpClients = GEditor->GetAllViewportClients();

		if (!vpClients.Contains(ActiveViewportClient)) ActiveViewportClient.Set(nullptr);

		// TODO: ignore camera movement when the player possesses a Pawn in PIE, but not when ejected or only SIE
		auto activeVpcCandidate = vpClients
			| FilterValid()
			| rv::filter([this](const FEditorViewportClient* cvp)
			{
				return cvp != ActiveViewportClient
					&& cvp->GetEditorViewportWidget().IsValid()
					&& cvp->GetEditorViewportWidget()->HasAnyUserFocusOrFocusedDescendants()
					&& cvp->IsVisible();
			});

		if (auto cvp = activeVpcCandidate | First(nullptr))
		{
			if (ActiveViewportClient.Get())
			{
				ActiveViewportClient->ToggleOrbitCamera(bVpWadOrbitCamera);
				ActiveViewportClient->SetRealtime(bVpWadRealtime);
			}
			bVpWadOrbitCamera = cvp->ShouldOrbitCamera();
			bVpWadRealtime = cvp->IsRealtime();
			ActiveViewportClient.Set(cvp);
		}

		IsFocused = ActiveViewportClient.Get()
			&& ActiveViewportClient->GetEditorViewportWidget()->HasAnyUserFocusOrFocusedDescendants();

		if (IsFocused.OnDown())
		{
			IsActive = true;
		}

		if (!ActiveViewportClient.Get() || !ActiveViewportClient->IsVisible())
		{
			IsActive = false;
			return;
		}

		if (IsActive)
		{
			auto&& manager = FSmEditorManager::Get();
			auto settings = GetMutableDefault<USpaceMouseConfig>();
			float camspeed = ActiveViewportClient->GetCameraSpeedSetting();

			if (!manager.IsLearning())
			{
				if (manager.GetButton(FSmInputDevice::GetButtonFrom(settings->DecreaseSpeedButton)).OnDown())
					ActiveViewportClient->SetCameraSpeedSetting(camspeed - 1);

				if (manager.GetButton(FSmInputDevice::GetButtonFrom(settings->IncreaseSpeedButton)).OnDown())
					ActiveViewportClient->SetCameraSpeedSetting(camspeed + 1);

				if (manager.GetButton(FSmInputDevice::GetButtonFrom(settings->ResetSpeedButton)).OnDown())
					ActiveViewportClient->SetCameraSpeedSetting(4);

				if (manager.GetButton(FSmInputDevice::GetButtonFrom(settings->ResetRollButton)).OnDown())
					ActiveViewportClient->RemoveCameraRoll();
			}

			auto& movementState = manager.MovementState;

			if (movementState.bOnMovementStartedFrame)
			{
				bVpWadRealtime = ActiveViewportClient->IsRealtime();
				bVpWadOrbitCamera = ActiveViewportClient->ShouldOrbitCamera();
				ActiveViewportClient->ToggleOrbitCamera(false);
				ActiveViewportClient->SetRealtime(true);
			}

			if (movementState.bOnMovementEndedFrame)
			{
				ActiveViewportClient->SetRealtime(bVpWadRealtime);
				//ActiveViewportClient->ToggleOrbitCamera(bVpWadOrbitCamera);
			}

		}

		IWidgetInteractionContext::Tick();

		if (AllowPerspectiveCameraMoveEvent(ActiveViewportClient))
		{
			// This is important to trigger PerspectiveCameraMoved event from outside.
			ActiveViewportClient->MoveViewportCamera(FVector::ZeroVector, FRotator::ZeroRotator);
		}
		ActiveViewportClient->Viewport->InvalidateHitProxy();
	}

	float FViewportInteraction::GetCameraSpeedMul() const
	{
		float speedExp = FMath::Max(ActiveViewportClient->GetCameraSpeedSetting() - 8, 0);
		speedExp += FMath::Min(ActiveViewportClient->GetCameraSpeedSetting(), 0);
		return FMath::Pow(2, speedExp) * ActiveViewportClient->GetCameraSpeed();
	}

	bool FViewportInteraction::AllowPerspectiveCameraMoveEvent(FEditorViewportClient* cvp)
	{
		static TSet IncompatibleViewports =
		{
			NAME_"SStaticMeshEditorViewport"
		};

		FName widgetType = cvp->GetEditorViewportWidget()->GetType();
		return !IncompatibleViewports.Contains(widgetType);
	}
}
