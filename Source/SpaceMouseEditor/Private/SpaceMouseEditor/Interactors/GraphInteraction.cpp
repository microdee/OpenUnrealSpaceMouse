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

#include "GraphInteraction.h"

#include "SGraphPanel.h"
#include "SpaceMouseEditor.h"
#include "SpaceMouseEditor/SmEditorManager.h"
#include "SpaceMouseEditor/SpaceMouseConfig.h"
#include "SpaceMouseEditor/Interactors/GraphEditorCustomizations.h"

namespace SpaceMouse::Editor::Interactor
{
	TModuleBoundObject<FSpaceMouseEditorModule, FGraphInteraction> GGraphInteraction {};

	FGraphInteraction& GetGraphInteraction()
	{
		return GGraphInteraction.GetChecked();
	}

	FGraphInteraction::FGraphInteraction()
	{
		RegisterContext();
	}

	TSharedPtr<SGraphPanel> FGraphInteraction::GetActiveGraphPanel() const
	{
		return ActiveGraphPanel.Pin();
	}

	bool FGraphInteraction::HasInstalledFractionalZoom() const
	{
		return bInstalledFractionalZoom;
	}

	void FGraphInteraction::Tick()
	{
		if (ShouldInitialize)
		{
			RegisterMouseWheelProcessor();
			SetInteraction(MakeShared<FGraphInteractionMode>());
		}

		auto settings = GetMutableDefault<USpaceMouseConfig>();
		if (!settings->bEnableBlueprintNavigation)
		{
			ActiveGraphPanel.Reset();
			IsActive = false;
			IsFocused = false;
			BlueprintZoomAccumulator = 0.0f;
			bInstalledFractionalZoom = false;
			CurrentDiscreteZoomLevel = FGraphNavZoomLevelsContainer::GetNearestDiscreteLevel(1.0f);
			return;
		}
		ManageActiveBlueprintGraph();

		if (IsFocused.OnDown())
		{
			IsActive = true;
		}

		IWidgetInteractionContext::Tick();
	}

	void FGraphInteraction::ManageActiveBlueprintGraph()
	{
		TSharedPtr<SGraphPanel> foundPanel = FindFocusedGraphPanel();

		if (foundPanel.IsValid())
		{
			// Reset accumulator when switching to a different panel
			if (ActiveGraphPanel.Pin() != foundPanel)
			{
				BlueprintZoomAccumulator = 0.0f;
				bInstalledFractionalZoom = false;
				// Initialize discrete level from current zoom
				float CurrentZoom = foundPanel->GetZoomAmount();
				CurrentDiscreteZoomLevel = FGraphNavZoomLevelsContainer::GetNearestDiscreteLevel(CurrentZoom);
			}
			ActiveGraphPanel = foundPanel;

			// Install fractional zoom if not already done
			if (!bInstalledFractionalZoom)
			{
				InstallFractionalZoom(foundPanel);
			}
		}
		IsFocused.Set(foundPanel.IsValid());
	}

	void FGraphInteraction::InstallFractionalZoom(TSharedPtr<SGraphPanel> const& graphPanel)
	{
		if (!graphPanel.IsValid())
		{
			return;
		}

		// Get the current zoom before replacing the container
		float currentZoom = graphPanel->GetZoomAmount();

		// Initialize discrete level from current zoom
		CurrentDiscreteZoomLevel = FGraphNavZoomLevelsContainer::GetNearestDiscreteLevel(currentZoom);

		// Install a custom fractional zoom container
		graphPanel->SetZoomLevelsContainer<FGraphNavZoomLevelsContainer>();

		// Restore the zoom level
		FVector2D CurrentOffset = graphPanel->GetViewOffset();
		graphPanel->RestoreViewSettings(CurrentOffset, currentZoom, FGuid());

		bInstalledFractionalZoom = true;
	}

	TSharedPtr<SGraphPanel> FGraphInteraction::FindFocusedGraphPanel()
	{
		if (!FSlateApplication::IsInitialized())
		{
			return nullptr;
		}

		auto&& slateApp = FSlateApplication::Get();
		TSharedPtr<SWidget> focusedWidget = slateApp.GetUserFocusedWidget(0);

		if (!focusedWidget.IsValid())
		{
			return nullptr;
		}

		// Check if focused widget is a graph panel
		if (focusedWidget->GetType() == NAME_"SGraphPanel")
		{
			return StaticCastSharedPtr<SGraphPanel>(focusedWidget);
		}

		// Search ancestors for graph panel
		TSharedPtr<SWidget> currentWidget = focusedWidget;
		while (currentWidget.IsValid())
		{
			TSharedPtr<SWidget> parentWidget = currentWidget->GetParentWidget();
			if (!parentWidget.IsValid())
			{
				break;
			}

			if (parentWidget->GetType() == NAME_"SGraphPanel")
			{
				return StaticCastSharedPtr<SGraphPanel>(parentWidget);
			}

			currentWidget = parentWidget;
		}

		return nullptr;
	}

	TSharedPtr<SGraphPanel> FGraphInteraction::FindGraphPanelInWidget(TSharedPtr<SWidget> const& widget)
	{
		if (!widget.IsValid())
		{
			return nullptr;
		}

		if (widget->GetType() == NAME_"SGraphPanel")
		{
			return StaticCastSharedPtr<SGraphPanel>(widget);
		}

		if (FChildren* children = widget->GetChildren())
		{
			for (int32 i = 0; i < children->Num(); ++i)
			{
				TSharedRef<SWidget> child = children->GetChildAt(i);
				TSharedPtr<SGraphPanel> found = FindGraphPanelInWidget(child);
				if (found.IsValid())
				{
					return found;
				}
			}
		}

		return nullptr;
	}

	void FGraphInteraction::RegisterMouseWheelProcessor()
	{

		if (!FSlateApplication::IsInitialized())
		{
			return;
		}

		if (!MouseWheelProcessor.IsValid())
		{
			MouseWheelProcessor = MakeShared<FGraphNavInputProcessor>();
			FSlateApplication::Get().RegisterInputPreProcessor(MouseWheelProcessor);
		}
	}

	void FGraphInteraction::UnregisterMouseWheelProcessor()
	{
		if (FSlateApplication::IsInitialized() && MouseWheelProcessor.IsValid())
		{
			FSlateApplication::Get().UnregisterInputPreProcessor(MouseWheelProcessor);
			MouseWheelProcessor.Reset();
		}
	}

	FGraphInteractionMode::FGraphInteractionMode()
	{
		ConstructInteractor();
	}

	void FGraphInteractionMode::Tick(IWidgetInteractionContext& ctx)
	{
		auto&& graphCtx = static_cast<FGraphInteraction&>(ctx);
		auto settings = GetMutableDefault<USpaceMouseConfig>();
		auto&& manager = FSmEditorManager::Get();
		auto trans = manager.GetNormalizedTranslation();
		auto rot = manager.GetNormalizedRotation();
		auto deltaSecs = FSlateApplication::Get().GetDeltaTime();

		if (trans.IsNearlyZero(SMALL_NUMBER) && rot.IsNearlyZero(SMALL_NUMBER))
		{
			return;
		}

		TSharedPtr<SGraphPanel> graphPanel = graphCtx.GetActiveGraphPanel();
		if (!graphPanel.IsValid())
		{
			return;
		}

		FGeometry panelGeometry = graphPanel->GetCachedGeometry();
		FVector2D panelSize = FVector2D(panelGeometry.GetLocalSize());

		// Get current view state
		FVector2D currentOffset = graphPanel->GetViewOffset();
		float currentZoom = graphPanel->GetZoomAmount();

		float panX = trans.Y * settings->BlueprintPanSpeed * panelSize.GetMax() * deltaSecs;
		float panY = settings->BlueprintPanningPlane == EOrthoSmPlane::LateralIsZoomVerticalIsUp
			? trans.Z * settings->BlueprintPanSpeed * panelSize.GetMax() * deltaSecs
			: trans.X * settings->BlueprintPanSpeed * panelSize.GetMax() * deltaSecs
		;

		if (settings->bBlueprintInvertPanX) panX = -panX;
		if (settings->bBlueprintInvertPanY) panY = -panY;

		// Apply pan (divide by zoom to make panning consistent at all zoom levels)
		FVector2D newOffset = currentOffset;
		newOffset.X -= panX / currentZoom;
		newOffset.Y += panY / currentZoom;

		float zoomInput = settings->BlueprintPanningPlane == EOrthoSmPlane::LateralIsZoomVerticalIsUp
			? trans.X * settings->BlueprintZoomSpeed * deltaSecs
			: trans.Z * settings->BlueprintZoomSpeed * deltaSecs
		;
		if (settings->bBlueprintInvertZoom) zoomInput = -zoomInput;

		// Apply zoom as a multiplier for smooth scaling
		float zoomMultiplier = 1.0f + zoomInput;
		float newZoom = currentZoom * zoomMultiplier;

		// Clamp to limits
		constexpr float minZoom = 0.1f;
		constexpr float maxZoom = 2.0f;
		newZoom = FMath::Clamp(newZoom, minZoom, maxZoom);

		bool zoomChanged = !FMath::IsNearlyEqual(newZoom, currentZoom, 0.0001f);

		// Zoom toward center of the panel
		if (zoomChanged && newZoom > 0.0f)
		{
			FVector2D panelCenter = panelSize * 0.5;

			// Convert center to graph space at current zoom
			FVector2D centerInGraph = newOffset + (panelCenter / currentZoom);

			// Adjust offset so center stays at same graph position after zoom change
			newOffset = centerInGraph - (panelCenter / newZoom);
		}

		// Apply the view settings - with our fractional zoom container, this is smooth!
		bool panChanged = !newOffset.Equals(currentOffset, 0.01f);
		if ((panChanged || zoomChanged) && newZoom > 0.0f)
		{
			graphPanel->RestoreViewSettings(newOffset, newZoom, FGuid());
		}
	}

	bool FGraphInteractionMode::IsValid(IWidgetInteractionContext& ctx)
	{
		return true;
	}
}
