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



#include "SpaceMouseEditor/SmEditorManager.h"
#include "SpaceMouseEditor/SmViewportOverlay.h"
#include "SpaceMouseEditor.h"
#include "SpaceMouseRuntime/SmInputDevice.h"
#include "SpaceMouseReader/MovementState.h"
#include "CommonBehaviors.h"

#include "CameraController.h"
#include "Editor.h"
#include "GameFramework/PlayerInput.h"
#include "SEditorViewport.h"
#include "EditorViewportClient.h"
#include "SpaceMouseConfig.h"
#include "HAL/PlatformApplicationMisc.h"
#include "Framework/Application/SlateApplication.h"

namespace SpaceMouse::Editor
{
	using namespace SpaceMouse::Runtime;

	TModuleBoundObject<FSpaceMouseEditorModule, FSmEditorManager> GEditorManager {};
	
	void FSmEditorManager::Tick(float DeltaSecs)
	{
		TickManager(DeltaSecs);

		if (ShouldInitialize)
		{
			auto& ibm = FInputBindingManager::Get();
			ibm.SaveInputBindings();
		}

		// TODO: ignore camera movement when the player possesses a Pawn in PIE, but not when ejected or only SIE

		ManageActiveViewport();
		ManageOrbitingOverlay();
		TriggerCustomButtons();
		MoveActiveViewport(GetTranslation(), GetRotation());
	
		if (bFinishLearning)
			bLearning = bFinishLearning = false;
	}

	void FSmEditorManager::ManageOrbitingOverlay()
	{
		auto settings = GetMutableDefault<USpaceMouseConfig>();
		if (MovementState.bOnMovementStartedFrame && ActiveViewportClient && settings->DisplayOrbitingPivot)
			OrbitingOverlay = MakeShared<FSmViewportOverlay>(ActiveViewportClient);
		
		if (MovementState.bOnMovementEndedFrame)
			OrbitingOverlay.Reset();
	}

	void FSmEditorManager::ManageActiveViewport()
	{
		namespace rv = ranges::views;
		
		decltype(auto) vpClients = GEditor->GetAllViewportClients();

		if (!vpClients.Contains(ActiveViewportClient)) ActiveViewportClient = nullptr;

		// TODO: handle different kinds of viewports with modular features
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
			if (ActiveViewportClient)
			{
				ActiveViewportClient->ToggleOrbitCamera(bWasOrbitCamera);
				ActiveViewportClient->SetRealtime(bWasRealtime);
			}
			bWasOrbitCamera = cvp->ShouldOrbitCamera();
			bWasRealtime = cvp->IsRealtime();
			ActiveViewportClient = cvp;
		}
	}

	void FSmEditorManager::TriggerCustomButtons()
	{
		if (!ActiveViewportClient) return;
		if (!ActiveViewportClient->IsVisible()) return;
	
		auto settings = GetMutableDefault<USpaceMouseConfig>();
		if (!settings->ActiveInBackground)
		{
			if (!FPlatformApplicationMisc::IsThisApplicationForeground()) return;
		}
	
		float camspeed = ActiveViewportClient->GetCameraSpeedSetting();
	
		if (!bLearning)
		{
			if (GetButton(FSmInputDevice::GetButtonFrom(settings->DecreaseSpeedButton)).OnDown())
				ActiveViewportClient->SetCameraSpeedSetting(camspeed - 1);
			
			if (GetButton(FSmInputDevice::GetButtonFrom(settings->IncreaseSpeedButton)).OnDown())
				ActiveViewportClient->SetCameraSpeedSetting(camspeed + 1);
			
			if (GetButton(FSmInputDevice::GetButtonFrom(settings->ResetSpeedButton)).OnDown())
				ActiveViewportClient->SetCameraSpeedSetting(4);
			
			if (GetButton(FSmInputDevice::GetButtonFrom(settings->ResetRollButton)).OnDown())
				ActiveViewportClient->RemoveCameraRoll();
			
			if (GetButton(FSmInputDevice::GetButtonFrom(settings->ShowSpaceMousePreferencesButton)).OnDown())
				settings->GoToSmConfig();
			
			if (GetButton(FSmInputDevice::GetButtonFrom(settings->ShowInputBindingsButton)).OnDown())
				settings->GoToInputBindings();
				
			// Editor actions have been "off-loaded" to Keyboard Shortcuts in Editor Preferences
		}
	}

	FVector FSmEditorManager::GetOrbitingPosDeltaOffset(FRotator rotDelta, float forwardDelta)
	{
		auto settings = GetMutableDefault<USpaceMouseConfig>();
		if (MovementState.bOnMovementStartedFrame)
		{
			auto world = ActiveViewportClient->GetWorld();
			FHitResult hit;
			float traceLength = settings->OrbitingLineTraceLength;
			FVector startpoint = ActiveViewportClient->GetViewLocation();
			FVector endpoint = startpoint +
				ActiveViewportClient->GetViewRotation().RotateVector({1, 0, 0}) * traceLength;

			FCollisionQueryParams ColQuery;
			ColQuery.bTraceComplex = true;

			bool traceResult = world->LineTraceSingleByChannel(
				hit, startpoint, endpoint, ECC_Visibility,
				Construct([](FCollisionQueryParams& _)
				{
					_.bTraceComplex = true;
				})
			);

			if (traceResult)
			{
				LastOrbitPivot = hit.ImpactPoint;
				LastOrbitDistance = hit.Distance;

				LastOrbitPivotView = LastOrbitPivot - ActiveViewportClient->GetViewLocation();
				LastOrbitPivotView = ActiveViewportClient->GetViewRotation().GetInverse().RotateVector(LastOrbitPivotView);
			}
			if (FMath::IsNearlyZero(LastOrbitDistance))
			{
				LastOrbitDistance = 300;
				LastOrbitPivotView = {LastOrbitDistance, 0, 0};

				LastOrbitPivot = ActiveViewportClient->GetViewLocation() +
					ActiveViewportClient->GetViewRotation().RotateVector(LastOrbitPivotView);
			}
		}
	
		LastOrbitDistance -= forwardDelta;
		LastOrbitPivotView.X -= forwardDelta;

		if (OrbitingOverlay) OrbitingOverlay->Draw(LastOrbitPivot, LastOrbitDistance);

		return USmCommonBehaviors::GetOrbitingTranslationDelta(
			LastOrbitPivotView,
			ActiveViewportClient->GetViewRotation(),
			rotDelta,
			LastOrbitDistance,
			settings->CameraBehavior == ESpaceMouseCameraBehavior::OrbitingWithRoll
		);
	}

	FSmUserSettings FSmEditorManager::GetUserSettings()
	{
		return GetMutableDefault<USpaceMouseConfig>()->GetUserSettings();
	}

	FSmEditorManager& FSmEditorManager::Get()
	{
		return GEditorManager.GetChecked();
	}

	void FSmEditorManager::BeginLearning()
	{
		bLearning = true;
		bFinishLearning = false;
	}

	void FSmEditorManager::EndLearning()
	{
		bFinishLearning = true;
	}

	FKeyEvent FSmEditorManager::GetKeyEventFromKey(const FInputActionKeyMapping& mapping)
	{
		const uint32* kc;
		const uint32* cc;
		FInputKeyManager::Get().GetCodesFromKey(mapping.Key, kc, cc);
	
		return FKeyEvent(
			mapping.Key, FModifierKeysState(
				mapping.bShift, false,
				mapping.bCtrl, false,
				mapping.bAlt, false,
				mapping.bCmd, false,
				false
			),
			0, false, cc ? *cc : 0, kc ? *kc : 0
		);
	}

	bool FSmEditorManager::AllowPerspectiveCameraMoveEvent(FEditorViewportClient* cvp)
	{
		// TODO: handle different kinds of viewports with modular features
		static TSet IncompatibleViewports =
		{
			NAME_"SStaticMeshEditorViewport"
		};

		FName widgetType = cvp->GetEditorViewportWidget()->GetType();
		return !IncompatibleViewports.Contains(widgetType);
	}

	void FSmEditorManager::MoveActiveViewport(FVector trans, FRotator rot)
	{
		if (!ActiveViewportClient) return;
		if (!ActiveViewportClient->IsVisible()) return;
	
		auto settings = GetMutableDefault<USpaceMouseConfig>();
		if (!settings->ActiveInBackground)
		{
			if (!FPlatformApplicationMisc::IsThisApplicationForeground()) return;
		}
	
		if (MovementState.bOnMovementStartedFrame && ActiveViewportClient)
		{
			bWasRealtime = ActiveViewportClient->IsRealtime();
			bWasOrbitCamera = ActiveViewportClient->ShouldOrbitCamera();
			ActiveViewportClient->ToggleOrbitCamera(false);
			ActiveViewportClient->SetRealtime(true);
		}

		if (MovementState.bOnMovementEndedFrame && ActiveViewportClient)
		{
			ActiveViewportClient->SetRealtime(bWasRealtime);
			//ActiveViewportClient->ToggleOrbitCamera(bWasOrbitCamera);
		}
	
		if (trans.IsNearlyZero(SMALL_NUMBER) && rot.IsNearlyZero(SMALL_NUMBER)) return;
	
		float speedExp = FMath::Max(ActiveViewportClient->GetCameraSpeedSetting() - 8, 0);
		speedExp += FMath::Min(ActiveViewportClient->GetCameraSpeedSetting(), 0);
		float speedMul = FMath::Pow(2, speedExp);
		speedMul *= ActiveViewportClient->GetCameraSpeed();

		bool orbitMovesObject = settings->OrbitingMovesObject;
		bool orbitRotatesObject = settings->OrbitingRotatesObject;
	
		if (ActiveViewportClient->IsPerspective())
		{
			FRotator currRot = ActiveViewportClient->GetViewRotation();

			if (settings->CameraBehavior >= ESpaceMouseCameraBehavior::OrbitingWithRoll)
			{
				auto orbRot = rot;
				if (settings->CameraBehavior == ESpaceMouseCameraBehavior::OrbitingNoRoll)
				{
					orbRot.Pitch *= currRot.Pitch > -80 && currRot.Pitch < 80;
				}
				auto orbitTrans = orbitMovesObject ? -trans : trans;
				trans = GetOrbitingPosDeltaOffset(
					orbitRotatesObject ? orbRot : orbRot.GetInverse(),
					orbitTrans.X * speedMul
				) / speedMul + orbitTrans;
			}
		
			FVector posDelta = currRot.RotateVector(trans) * speedMul;

			FVector currPos = ActiveViewportClient->GetViewLocation();
			currPos += posDelta;
			switch (settings->CameraBehavior)
			{
			case ESpaceMouseCameraBehavior::CameraDeltaWithRoll:
				currRot = FRotator(FQuat(currRot) * FQuat(rot));
				break;
			
			case ESpaceMouseCameraBehavior::CameraDeltaNoRoll:
				if (MovementState.bOnMovementStartedFrame)
					ActiveViewportClient->RemoveCameraRoll();
				
				currRot.Pitch = FMath::Clamp(currRot.Pitch, -80.0f, 80.0f);
				currRot = FRotator(
					FQuat(FRotator(0, rot.Yaw, 0)) *
					FQuat(currRot) *
					FQuat(FRotator(rot.Pitch, 0, 0))
				);
				currRot.Roll = 0;
				break;
			
			case ESpaceMouseCameraBehavior::OrbitingWithRoll:
				currRot = FRotator(FQuat(currRot) * (orbitRotatesObject ? FQuat(rot).Inverse() : FQuat(rot)));
				break;
			
			case ESpaceMouseCameraBehavior::OrbitingNoRoll:
				if (MovementState.bOnMovementStartedFrame)
					ActiveViewportClient->RemoveCameraRoll();
				
				currRot.Pitch = FMath::Clamp(currRot.Pitch, -80.0f, 80.0f);
				currRot = FRotator(
					FQuat(FRotator(0, orbitRotatesObject ? -rot.Yaw : rot.Yaw, 0))
					* FQuat(currRot)
					* FQuat(FRotator(orbitRotatesObject ? -rot.Pitch : rot.Pitch, 0, 0))
				);
				currRot.Roll = 0;
				break;
			default: ;
			}
			ActiveViewportClient->SetViewLocation(currPos);
			ActiveViewportClient->SetViewRotation(currRot);
		}
		else if (ActiveViewportClient->IsOrtho())
		{
			float currZoom = ActiveViewportClient->GetOrthoZoom();
			float currZoomSpeed = currZoom / settings->TranslationUnitsPerSec * 0.25;
			float zoomDelta = settings->OrthoPanningPlane == EOrthoSmPlane::LateralIsZoomVerticalIsUp
				? trans.X : -trans.Z;
		
			zoomDelta *= speedMul * currZoomSpeed * settings->OrthoZoomSpeed * 8;
			if (settings->OrbitingMovesObject) zoomDelta *= -1;
			currZoom -= zoomDelta;
		
			if (currZoom < 1) currZoom = 1;
		
			FVector currPos = ActiveViewportClient->ViewTransformOrthographic.GetLocation();
			FRotator currRot;
			switch (ActiveViewportClient->GetViewportType())
			{
			case LVT_OrthoXY:
				{
					currRot = FRotationMatrix::MakeFromX({0, 0, -1}).Rotator();
					currRot = FRotator(currRot.Quaternion() * FRotator(0, 0, -90).Quaternion());
					break;
				}
			case LVT_OrthoXZ: currRot = FRotationMatrix::MakeFromX({0, -1, 0}).Rotator(); break;
			case LVT_OrthoYZ: currRot = FRotationMatrix::MakeFromX({1, 0, 0}).Rotator(); break;
			case LVT_OrthoFreelook:
				{
					// TODO
					currRot = {}; break;
				}
			case LVT_OrthoNegativeXY: 
				{
					currRot = FRotationMatrix::MakeFromX({0, 0, 1}).Rotator();
					currRot = FRotator(currRot.Quaternion() * FRotator(0, 0, 90).Quaternion());
					break;
				}
			case LVT_OrthoNegativeXZ: currRot = FRotationMatrix::MakeFromX({0, 1, 0}).Rotator(); break;
			case LVT_OrthoNegativeYZ: currRot = FRotationMatrix::MakeFromX({-1, 0, 0}).Rotator(); break;
			default: ;
			}
		
			FVector orthoTrans;

			switch (settings->OrthoPanningPlane)
			{
			case EOrthoSmPlane::LateralIsZoomVerticalIsUp: orthoTrans = {0, trans.Y, trans.Z}; break;
			case EOrthoSmPlane::LateralIsUpVerticalIsZoom: orthoTrans = {0, trans.Y, trans.X}; break;
			}
		
			FVector posDelta = currRot.RotateVector(orthoTrans) * speedMul * currZoomSpeed;
			if (settings->OrbitingMovesObject) posDelta *= -1;
			currPos += posDelta;

			ActiveViewportClient->ViewTransformOrthographic.SetLocation(currPos);
			ActiveViewportClient->SetOrthoZoom(currZoom);
		}
		if (AllowPerspectiveCameraMoveEvent(ActiveViewportClient))
		{
			// This is important to trigger PerspectiveCameraMoved event from outside.
			ActiveViewportClient->MoveViewportCamera(FVector::ZeroVector, FRotator::ZeroRotator);
		}
		ActiveViewportClient->Viewport->InvalidateHitProxy();
	}
}
