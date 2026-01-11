/** @noop License Comment
 *  @file
 *  @copyright
 *  This Source Code is subject to the terms of the Mozilla Public License, v2.0.
 *  If a copy of the MPL was not distributed with this file You can obtain one at
 *  https://mozilla.org/MPL/2.0/
 *
 *  @author Max Litruv Boonzaayer, David Mórász
 *  @date 2025
 */


#pragma once

#include "CoreMinimal.h"
#include "Framework/Application/IInputProcessor.h"
#include "SNodePanel.h"

namespace SpaceMouse::Editor::Interactor
{
	/**
	 * Input preprocessor that intercepts mouse wheel events for blueprint graph zoom.
	 */
	class SPACEMOUSEEDITOR_API FGraphNavInputProcessor : public IInputProcessor
	{
		virtual void Tick(const float deltaSecs, FSlateApplication& slateApp, TSharedRef<ICursor> cursor) override {}
		virtual bool HandleMouseWheelOrGestureEvent(FSlateApplication& slateApp, const FPointerEvent& wheelEvent, const FPointerEvent* gestureEvent) override;
	};

	/**
	 * Custom zoom levels container that allows truly fractional/continuous zooming.
	 * Instead of fixed discrete levels, this stores the current zoom as a float
	 * and returns it directly, allowing smooth zooming.
	 */
	struct SPACEMOUSEEDITOR_API FGraphNavZoomLevelsContainer : public FZoomLevelsContainer
	{
		mutable float CurrentZoom = 1.0f;

		static constexpr float MinZoom = 0.1f;
		static constexpr float MaxZoom = 2.0f;
		static constexpr int32 NumZoomLevels = 20;

		/** Fixed discrete zoom levels for mouse wheel
		 * (from SNodePanel::FFixedZoomLevelsContainer)
		*/
		static constexpr float DiscreteZoomLevels[NumZoomLevels] = {
			0.100f,
			0.125f,
			0.150f,
			0.175f,
			0.200f,
			0.225f,
			0.250f,
			0.375f,
			0.500f,
			0.675f,
			0.750f,
			0.875f,
			1.000f,
			1.250f,
			1.375f,
			1.500f,
			1.675f,
			1.750f,
			1.875f,
			2.000f
		};

		/** Get the zoom value for a discrete zoom level index. */
		static float GetDiscreteZoomLevel(int32 level);

		/** Get the nearest discrete zoom level index for a given zoom amount. */
		static int32 GetNearestDiscreteLevel(float zoomAmount);

		FORCEINLINE void SetZoom(float zoom) const { CurrentZoom = FMath::Clamp(zoom, MinZoom, MaxZoom); }

		virtual float GetZoomAmount(int32 zoomLevel) const override;
		virtual int32 GetNearestZoomLevel(float zoomAmount) const override;
		virtual FText GetZoomText(int32 zoomLevel) const override;
		FORCEINLINE virtual int32 GetNumZoomLevels() const override { return 1; }
		FORCEINLINE virtual int32 GetDefaultZoomLevel() const override { return 0; }
		virtual EGraphRenderingLOD::Type GetLOD(int32 zoomLevel) const override;
	};
}
