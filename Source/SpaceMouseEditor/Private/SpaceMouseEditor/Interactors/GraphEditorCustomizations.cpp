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

#include "GraphEditorCustomizations.h"

#include "SGraphPanel.h"
#include "HAL/PlatformApplicationMisc.h"
#include "Framework/Application/SlateApplication.h"
#include "SpaceMouseEditor/Interactors/GraphInteraction.h"
#include "SpaceMouseEditor/SpaceMouseConfig.h"

namespace SpaceMouse::Editor::Interactor
{
	bool FGraphNavInputProcessor::HandleMouseWheelOrGestureEvent(FSlateApplication& slateApp, const FPointerEvent& wheelEvent, const FPointerEvent* gestureEvent)
	{
		float wheelDelta = wheelEvent.GetWheelDelta();
		if (FMath::IsNearlyZero(wheelDelta))
		{
			return false;
		}

		bool ctrlDown = wheelEvent.IsControlDown();
		auto settings = GetMutableDefault<USpaceMouseConfig>();
		auto screenSpacePosition = wheelEvent.GetScreenSpacePosition();
		if (!settings->bEnableBlueprintNavigation)
		{
			return false;
		}

		if (!settings->ActiveInBackground && !FPlatformApplicationMisc::IsThisApplicationForeground())
		{
				return false;
		}

		if (!FSlateApplication::IsInitialized())
		{
			return false;
		}

		FWidgetPath widgetPath = slateApp.LocateWindowUnderMouse(screenSpacePosition, slateApp.GetInteractiveTopLevelWindows());

		TSharedPtr<SGraphPanel> graphPanel;
		for (int32 i = widgetPath.Widgets.Num() - 1; i >= 0; --i)
		{
			TSharedRef<SWidget> widget = widgetPath.Widgets[i].Widget;
			if (widget->GetType() == NAME_"SGraphPanel")
			{
				graphPanel = StaticCastSharedRef<SGraphPanel>(widget);
				break;
			}
		}

		if (!graphPanel.IsValid())
		{
			return false;
		}

		auto&& graphInteraction = GetGraphInteraction();

		// Only handle if we've already installed fractional zoom on this panel (from ManageActiveBlueprintGraph)
		if (graphInteraction.GetActiveGraphPanel() != graphPanel || !graphInteraction.HasInstalledFractionalZoom())
		{
			// Let default Slate handling occur - we haven't taken over this panel yet
			return false;
		}

		// Get current zoom state
		float currentZoom = graphPanel->GetZoomAmount();
		FVector2D currentOffset = graphPanel->GetViewOffset();

		// Require Ctrl to zoom in beyond 100%; zooming out is always allowed
		const int32 zoomLevel100Percent = FGraphNavZoomLevelsContainer::GetNearestDiscreteLevel(1.0f);

		// Apply discrete zoom step based on wheel direction
		if (wheelDelta > 0)
		{
			// Scroll up = zoom in = higher level
			int32 nextLevel = graphInteraction.CurrentDiscreteZoomLevel + 1;
			if (!ctrlDown && nextLevel > zoomLevel100Percent)
			{
				// At or above 100%, wheel-up without Ctrl does nothing
				nextLevel = graphInteraction.CurrentDiscreteZoomLevel;
			}
			graphInteraction.CurrentDiscreteZoomLevel = FMath::Clamp(nextLevel, 0, FGraphNavZoomLevelsContainer::NumZoomLevels - 1);
		}
		else
		{
			// Scroll down = zoom out = lower level (no Ctrl required)
			graphInteraction.CurrentDiscreteZoomLevel = FMath::Max(graphInteraction.CurrentDiscreteZoomLevel - 1, 0);
		}

		// Get the target zoom for this discrete level
		float newZoom = FGraphNavZoomLevelsContainer::GetDiscreteZoomLevel(graphInteraction.CurrentDiscreteZoomLevel);

		// Get local mouse position for zoom-toward-cursor
		FGeometry panelGeometry = graphPanel->GetCachedGeometry();
		FVector2D localMousePos = panelGeometry.AbsoluteToLocal(screenSpacePosition);

		// Convert zoom point to graph space at current zoom
		FVector2D pointInGraph = currentOffset + (localMousePos / currentZoom);

		// Adjust offset so point stays at same graph position after zoom
		FVector2D newOffset = pointInGraph - (localMousePos / newZoom);

		graphPanel->RestoreViewSettings(newOffset, newZoom, FGuid());

		// We handled this event - consume it so Slate doesn't process it too
		return true;
	}

	float FGraphNavZoomLevelsContainer::GetDiscreteZoomLevel(int32 level)
	{
		level = FMath::Clamp(level, 0, NumZoomLevels - 1);
		return DiscreteZoomLevels[level];
	}

	int32 FGraphNavZoomLevelsContainer::GetNearestDiscreteLevel(float zoomAmount)
	{
		zoomAmount = FMath::Clamp(zoomAmount, MinZoom, MaxZoom);
		int32 nearestLevel = 0;
		float smallestDiff = FMath::Abs(zoomAmount - DiscreteZoomLevels[0]);
		for (int32 i = 1; i < NumZoomLevels; ++i)
		{
			float Diff = FMath::Abs(zoomAmount - DiscreteZoomLevels[i]);
			if (Diff < smallestDiff)
			{
				smallestDiff = Diff;
				nearestLevel = i;
			}
		}
		return nearestLevel;
	}

	float FGraphNavZoomLevelsContainer::GetZoomAmount(int32 zoomLevel) const
	{
		return CurrentZoom;
	}

	int32 FGraphNavZoomLevelsContainer::GetNearestZoomLevel(float zoomAmount) const
	{
		CurrentZoom = FMath::Clamp(zoomAmount, MinZoom, MaxZoom);
		return 0;
	}

	FText FGraphNavZoomLevelsContainer::GetZoomText(int32 zoomLevel) const
	{
		return INVTEXT_"Zoom {0}%" _FMT(FMath::RoundToInt(CurrentZoom * 100.f));
	}

	EGraphRenderingLOD::Type FGraphNavZoomLevelsContainer::GetLOD(int32 zoomLevel) const
	{
		if (CurrentZoom <= 0.20f) return EGraphRenderingLOD::LowestDetail;
		if (CurrentZoom <= 0.25f) return EGraphRenderingLOD::LowDetail;
		if (CurrentZoom <= 0.675f) return EGraphRenderingLOD::MediumDetail;
		if (CurrentZoom <= 1.375f) return EGraphRenderingLOD::DefaultDetail;
		return EGraphRenderingLOD::FullyZoomedIn;
	}
}
