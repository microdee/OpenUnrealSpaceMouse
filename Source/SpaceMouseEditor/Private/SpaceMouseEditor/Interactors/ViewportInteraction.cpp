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

#include "LevelEditorViewport.h"
#include "OrthoViewportMode.h"
#include "PerspectiveViewportMode.h"
#include "SLevelViewport.h"
#include "SpaceMouseEditor.h"
#include "Framework/Application/SlateApplication.h"
#include "HAL/PlatformApplicationMisc.h"
#include "SpaceMouseEditor/SmEditorManager.h"
#include "SpaceMouseEditor/SpaceMouseConfig.h"
#include "SpaceMouseRuntime/RuntimeManager.h"
#include "SpaceMouseRuntime/SmInputDevice.h"

namespace SpaceMouse::Editor::Interactor
{
	TModuleBoundObject<FSpaceMouseEditorModule, FViewportInteraction> GViewportInteraction {};

	bool FViewportMouseWheelSpeedChange::HandleMouseWheelOrGestureEvent(FSlateApplication& slateApp, const FPointerEvent& wheelEvent, const FPointerEvent* gestureEvent)
	{
		float delta = wheelEvent.GetWheelDelta();
		if (!FMath::IsNearlyZero(delta))
		{
			GViewportInteraction.GetChecked().ChangeCameraSpeed(delta * 0.1f);
			return true;
		}
		return false;
	}

	FViewportInteraction::FViewportInteraction()
	{
		RegisterContext();
		MouseWheelSpeedChange = MakeShared<FViewportMouseWheelSpeedChange>();
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

		if (!vpClients.Contains(ActiveViewportClient))
		{
			ActiveViewportClient.Set(nullptr);
		}

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
			cvp = cvp->IsLevelEditorClient() ? GCurrentLevelEditingViewportClient : cvp;
			if (ActiveViewportClient.Get() != cvp)
			{
				if (ActiveViewportClient.Get())
				{
					ActiveViewportClient->SetRealtime(bVpWasRealtime);
					// ActiveViewportClient->ToggleOrbitCamera(bVpWasOrbitCamera);
				}
				// bVpWasOrbitCamera = cvp->ShouldOrbitCamera();
				bVpWasRealtime = cvp->IsRealtime();
				ActiveViewportClient.Set(cvp);
			}
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

			if (!manager.IsLearning())
			{
				if (manager.GetButton(FSmInputDevice::GetButtonFrom(settings->DecreaseSpeedButton)).OnDown())
					ChangeCameraSpeed(-0.5);

				if (manager.GetButton(FSmInputDevice::GetButtonFrom(settings->IncreaseSpeedButton)).OnDown())
					ChangeCameraSpeed(0.5);

				if (manager.GetButton(FSmInputDevice::GetButtonFrom(settings->ResetSpeedButton)).OnDown())
					ResetSpeed();

				if (manager.GetButton(FSmInputDevice::GetButtonFrom(settings->ResetRollButton)).OnDown())
					ActiveViewportClient->RemoveCameraRoll();
			}

			auto& movementState = manager.MovementState;

			if (movementState.bOnMovementStartedFrame)
			{
				bVpWasRealtime = ActiveViewportClient->IsRealtime();
				ActiveViewportClient->ToggleOrbitCamera(false);
				ActiveViewportClient->SetRealtime(true);

				// Set scroll wheel speed change
				FSlateApplication::Get().RegisterInputPreProcessor(MouseWheelSpeedChange);
			}

			if (movementState.bOnMovementEndedFrame)
			{
				// ActiveViewportClient->ToggleOrbitCamera(bVpWasOrbitCamera);
				ActiveViewportClient->SetRealtime(bVpWasRealtime);

				// Unset scroll wheel speed change
				FSlateApplication::Get().UnregisterInputPreProcessor(MouseWheelSpeedChange);
			}

		}

		IWidgetInteractionContext::Tick();

		if (ActiveViewportClient.Get() && ActiveViewportClient->IsLevelEditorClient())
		{
			auto levelVpc = static_cast<FLevelEditorViewportClient*>(ActiveViewportClient.Get());
			if (levelVpc->IsLockedToCinematic() || levelVpc->IsAnyActorLocked())
			{
				levelVpc->MoveViewportCamera(FVector::ZeroVector, FRotator::ZeroRotator);
			}
		}
		ActiveViewportClient->Viewport->InvalidateHitProxy();
	}

	void FViewportInteraction::ResetSpeed()
	{
#if UNREAL_VERSION(>=, 5, 7)
		ActiveViewportClient->SetCameraSpeedSettings({1.0});
#else
		ActiveViewportClient->SetCameraSpeedSetting(4);
#endif
	}

	void FViewportInteraction::ChangeCameraSpeed(float deltaCoeff)
	{
		if (!ActiveViewportClient.Get()) return;

#if UNREAL_VERSION(>=, 5, 7)
		FEditorViewportCameraSpeedSettings speedSettings = ActiveViewportClient->GetCameraSpeedSettings();
		const float speed = speedSettings.GetCurrentSpeed();
		speedSettings.SetCurrentSpeed(deltaCoeff < 0
			? speed * (1.f - deltaCoeff * -1.f)
			: speed / (1.f - deltaCoeff)
		);
		ActiveViewportClient->SetCameraSpeedSettings(speedSettings);
#else
		int speed = FMath::Clamp(ActiveViewportClient->GetCameraSpeedSetting() + FMath::Sign(deltaCoeff), 1, 8);
		ActiveViewportClient->SetCameraSpeedSetting(speed);
#endif
	}
}
