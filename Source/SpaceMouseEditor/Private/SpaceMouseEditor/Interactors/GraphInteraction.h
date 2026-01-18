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
#include "SpaceMouseEditor/Interactors/Interactor.h"

class SGraphPanel;

namespace SpaceMouse::Editor::Interactor
{
	class FGraphInteraction;
	class FGraphNavInputProcessor;

	SPACEMOUSEEDITOR_API FGraphInteraction& GetGraphInteraction();

	class SPACEMOUSEEDITOR_API FGraphInteraction
		: public IWidgetInteractionContext
		, public IFeatureImplementation
	{
	public:
		FGraphInteraction();

		TSharedPtr<SGraphPanel> GetActiveGraphPanel() const;
		bool HasInstalledFractionalZoom() const;

		int32 CurrentDiscreteZoomLevel = 12;  // Default to 100% (index 12 of 0-19)
		FBool IsFocused { false };
		virtual void Tick() override;

	private:
		FOnce ShouldInitialize;
		TWeakPtr<SGraphPanel> ActiveGraphPanel;
		float BlueprintZoomAccumulator = 0.0f;
		bool bInstalledFractionalZoom = false;
		TSharedPtr<FGraphNavInputProcessor> MouseWheelProcessor;

		void ManageActiveBlueprintGraph();
		void InstallFractionalZoom(TSharedPtr<SGraphPanel> const& graphPanel);
		static TSharedPtr<SGraphPanel> FindFocusedGraphPanel();
		static TSharedPtr<SGraphPanel> FindGraphPanelInWidget(TSharedPtr<SWidget> const& widget);
		void RegisterMouseWheelProcessor();
		void UnregisterMouseWheelProcessor();
	};

	class SPACEMOUSEEDITOR_API FGraphInteractionMode : public IWidgetInteractionMode
	{
	public:
		FGraphInteractionMode();
		virtual void Tick(IWidgetInteractionContext& ctx) override;
		virtual bool IsValid(IWidgetInteractionContext& ctx) override;
	};
}
