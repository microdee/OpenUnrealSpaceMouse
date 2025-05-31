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




#include "SmViewportOverlay.h"
#include "Editor.h"
#include "SEditorViewport.h"
#include "EditorViewportClient.h"
#include "SceneView.h"
#include "Widgets/SViewport.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/SWidget.h"
#include "Mcro/Common.h"

namespace SpaceMouse::Editor
{
	// TODO: Pray to your god that it ain't gonna dangle
	FSmViewportOverlay::FSmViewportOverlay(FEditorViewportClient* vpClient)
		: ViewportClient(vpClient)
	{
		auto vpRootWidget = vpClient->GetEditorViewportWidget();
		Viewport = StaticCastSharedPtr<SViewport>(GetChildWidgetOfType(vpRootWidget, TEXT_"SViewport"));
		if(!Viewport) return;
		Overlay = StaticCastSharedRef<SOverlay>(Viewport->GetChildren()->GetChildAt(0));

		Overlay->AddSlot()
			. HAlign(HAlign_Left)
			. VAlign(VAlign_Top)
			[
				SAssignNew(DistanceText, STextBlock)
				. SimpleTextMode(true)
				. ColorAndOpacity(FLinearColor::Yellow)
				. ShadowOffset(FVector2D(2, 2))
				. ShadowColorAndOpacity(FLinearColor::Black)
			];
	}

	FSmViewportOverlay::~FSmViewportOverlay()
	{
		if(Overlay)
			Overlay->RemoveSlot(DistanceText.ToSharedRef());
	}

	void FSmViewportOverlay::Draw(FVector pivot, float pivotDistance) const
	{
		FSceneViewFamilyContext viewFam({
			ViewportClient->Viewport,
			ViewportClient->GetScene(),
			ViewportClient->EngineShowFlags
		});
		auto dpiScale = ViewportClient->ShouldDPIScaleSceneCanvas() ? ViewportClient->GetDPIScale() : 1.0f;
		auto sceneView = ViewportClient->CalcSceneView(&viewFam);
		FVector2D pos2;
		auto vpSize = ViewportClient->Viewport->GetSizeXY();
		sceneView->ProjectWorldToScreen(
			pivot,
			FIntRect({0, 0}, vpSize),
			sceneView->ViewMatrices.GetViewProjectionMatrix(),
			pos2
		);
		
		DistanceText->SetText(FText::FromString(PRINTF_(pivotDistance) "%.2f cm"));

		pos2 /= dpiScale;
		pos2 -= DistanceText->GetDesiredSize() / 2.0f;
		DistanceText->SetRenderTransform(FSlateRenderTransform(pos2));
	}

	TSharedPtr<SWidget> FSmViewportOverlay::GetChildWidgetOfType(TSharedPtr<SWidget> const& widget, FName const& type)
	{
		if(widget->GetType() == type) return widget;
		auto children = widget->GetChildren();

		for(int i=0; i<children->Num(); i++)
		{
			auto CurrChild = GetChildWidgetOfType(
				children->GetChildAt(i),
				type
			);
			if(CurrChild.IsValid()) return CurrChild;
		}
		return {};
	}
}
