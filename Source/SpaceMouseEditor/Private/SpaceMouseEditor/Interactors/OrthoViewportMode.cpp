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


#include "OrthoViewportMode.h"

#include "SpaceMouseEditor/SmEditorManager.h"
#include "SpaceMouseEditor/SpaceMouseConfig.h"
#include "EditorViewportClient.h"

namespace SpaceMouse::Editor::Interactor
{
	FOrthoViewportMode::FOrthoViewportMode()
	{
		ConstructInteractor();
	}

	void FOrthoViewportMode::Tick(IWidgetInteractionContext& ctx)
	{
		auto&& vpCtx = static_cast<FViewportInteraction&>(ctx);
		auto vpClient = vpCtx.ActiveViewportClient.Get();
		auto&& manager = FSmEditorManager::Get();
		auto& movementState = manager.MovementState;
		auto trans = manager.GetTranslation();
		auto rot = manager.GetRotation();
		float speedMul = vpClient->GetCameraSpeed();
		auto settings = GetMutableDefault<USpaceMouseConfig>();

		float currZoom = vpClient->GetOrthoZoom();
		float currZoomSpeed = currZoom / settings->TranslationUnitsPerSec * 0.25;
		float zoomDelta = settings->OrthoPanningPlane == EOrthoSmPlane::LateralIsZoomVerticalIsUp
			? trans.X : -trans.Z;

		zoomDelta *= speedMul * currZoomSpeed * settings->OrthoZoomSpeed * 8;
		if (settings->OrbitingMovesObject) zoomDelta *= -1;
		currZoom -= zoomDelta;

		if (currZoom < 1) currZoom = 1;

		FVector currPos = vpClient->ViewTransformOrthographic.GetLocation();
		FRotator currRot;
		switch (vpClient->GetViewportType())
		{
		case LVT_OrthoXY:
			{
				currRot = FRotationMatrix::MakeFromX({0, 0, -1}).Rotator();
#if UNREAL_VERSION(<, 5, 6)
				currRot = FRotator(currRot.Quaternion() * FRotator(0, 0, -90).Quaternion());
#else
				currRot = FRotator(currRot.Quaternion() * FRotator(0, 0, -180).Quaternion());
#endif
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
#if UNREAL_VERSION(<, 5, 6)
				currRot = FRotator(currRot.Quaternion() * FRotator(0, 0, 90).Quaternion());
#else
				currRot = FRotator(currRot.Quaternion() * FRotator(0, 0, 180).Quaternion());
#endif
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

		vpClient->ViewTransformOrthographic.SetLocation(currPos);
		vpClient->SetOrthoZoom(currZoom);
	}

	bool FOrthoViewportMode::IsValid(IWidgetInteractionContext& ctx)
	{
		auto&& vpCtx = static_cast<FViewportInteraction&>(ctx);
		auto vpClient = vpCtx.ActiveViewportClient.Get();

		return vpClient->IsOrtho();
	}
}
