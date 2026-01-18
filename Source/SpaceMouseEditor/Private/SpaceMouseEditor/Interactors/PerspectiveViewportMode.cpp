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

#include "PerspectiveViewportMode.h"

#include "CommonBehaviors.h"
#include "EditorViewportClient.h"
#include "SpaceMouseEditor/SmEditorManager.h"
#include "SpaceMouseEditor/SmViewportOverlay.h"
#include "SpaceMouseEditor/SpaceMouseConfig.h"
#include "Framework/Application/SlateApplication.h"
#include "Engine/HitResult.h"

namespace SpaceMouse::Editor::Interactor
{
	FPerspectiveViewportMode::FPerspectiveViewportMode()
	{
		ConstructInteractor();
	}

	void FPerspectiveViewportMode::Tick(IWidgetInteractionContext& ctx)
	{
		auto&& vpCtx = static_cast<FViewportInteraction&>(ctx);
		auto vpClient = vpCtx.ActiveViewportClient.Get();
		auto&& manager = FSmEditorManager::Get();
		auto& movementState = manager.MovementState;
		auto trans = manager.GetTranslation();
		auto rot = manager.GetNormalizedRotation();
		float speedMul = vpCtx.GetCameraSpeedMul();
		auto settings = GetMutableDefault<USpaceMouseConfig>();
		auto deltaSecs = FSlateApplication::Get().GetDeltaTime();

		switch (settings->CameraBehavior)
		{
		case ESpaceMouseCameraBehavior::CameraDeltaWithRoll:
		case ESpaceMouseCameraBehavior::CameraDeltaNoRoll:
			rot.Pitch *= vpClient->ViewFOV * settings->RotationScreensPerSec * deltaSecs;
			rot.Yaw *= vpClient->ViewFOV * settings->RotationScreensPerSec * deltaSecs;
			rot.Roll *= settings->RotationDegreesPerSec * deltaSecs;
			break;
		case ESpaceMouseCameraBehavior::OrbitingWithRoll:
		case ESpaceMouseCameraBehavior::OrbitingNoRoll:
			rot *= settings->RotationDegreesPerSec * deltaSecs;
			break;
		}


		if (movementState.bOnMovementStartedFrame && settings->DisplayOrbitingPivot)
			OrbitingOverlay = MakeShared<FSmViewportOverlay>(vpClient);

		if (movementState.bOnMovementEndedFrame)
			OrbitingOverlay.Reset();

		bool orbitMovesObject = settings->OrbitingMovesObject;
		bool orbitRotatesObject = settings->OrbitingRotatesObject;

		FRotator currRot = vpClient->GetViewRotation();

		if (settings->CameraBehavior >= ESpaceMouseCameraBehavior::OrbitingWithRoll)
		{
			auto orbRot = rot;
			if (settings->CameraBehavior == ESpaceMouseCameraBehavior::OrbitingNoRoll)
			{
				orbRot.Pitch *= currRot.Pitch > -80 && currRot.Pitch < 80;
			}
			auto orbitTrans = orbitMovesObject ? -trans : trans;
			trans = GetOrbitingPosDeltaOffset(
				vpCtx,
				orbitRotatesObject ? orbRot : orbRot.GetInverse(),
				orbitTrans.X * speedMul
			) / speedMul + orbitTrans;
		}

		FVector posDelta = currRot.RotateVector(trans) * speedMul;

		FVector currPos = vpClient->GetViewLocation();
		currPos += posDelta;
		switch (settings->CameraBehavior)
		{
		case ESpaceMouseCameraBehavior::CameraDeltaWithRoll:
			currRot = FRotator(FQuat(currRot) * FQuat(rot));
			break;

		case ESpaceMouseCameraBehavior::CameraDeltaNoRoll:
			if (movementState.bOnMovementStartedFrame)
				vpClient->RemoveCameraRoll();

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
			if (movementState.bOnMovementStartedFrame)
				vpClient->RemoveCameraRoll();

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
		vpClient->SetViewLocation(currPos);
		vpClient->SetViewRotation(currRot);
	}

	bool FPerspectiveViewportMode::IsValid(IWidgetInteractionContext& ctx)
	{
		auto&& vpCtx = static_cast<FViewportInteraction&>(ctx);
		auto vpClient = vpCtx.ActiveViewportClient.Get();

		return vpClient->IsPerspective();
	}

	FVector FPerspectiveViewportMode::GetOrbitingPosDeltaOffset(FViewportInteraction& ctx, FRotator rotDelta, float forwardDelta)
	{
		auto vpClient = ctx.ActiveViewportClient.Get();
		auto&& manager = FSmEditorManager::Get();
		auto& movementState = manager.MovementState;

		auto settings = GetMutableDefault<USpaceMouseConfig>();
		if (movementState.bOnMovementStartedFrame)
		{
			auto world = vpClient->GetWorld();
			FHitResult hit;
			float traceLength = settings->OrbitingLineTraceLength;
			FVector startpoint = vpClient->GetViewLocation();
			FVector endpoint = startpoint +
				vpClient->GetViewRotation().RotateVector({1, 0, 0}) * traceLength;

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

				LastOrbitPivotView = LastOrbitPivot - vpClient->GetViewLocation();
				LastOrbitPivotView = vpClient->GetViewRotation().GetInverse().RotateVector(LastOrbitPivotView);
			}
			if (FMath::IsNearlyZero(LastOrbitDistance))
			{
				LastOrbitDistance = 300;
				LastOrbitPivotView = {LastOrbitDistance, 0, 0};

				LastOrbitPivot = vpClient->GetViewLocation() +
					vpClient->GetViewRotation().RotateVector(LastOrbitPivotView);
			}
		}

		LastOrbitDistance -= forwardDelta;
		LastOrbitPivotView.X -= forwardDelta;

		if (OrbitingOverlay) OrbitingOverlay->Draw(LastOrbitPivot, LastOrbitDistance);

		return USmCommonBehaviors::GetOrbitingTranslationDelta(
			LastOrbitPivotView,
			vpClient->GetViewRotation(),
			rotDelta,
			LastOrbitDistance,
			settings->CameraBehavior == ESpaceMouseCameraBehavior::OrbitingWithRoll
		);
	}
}
