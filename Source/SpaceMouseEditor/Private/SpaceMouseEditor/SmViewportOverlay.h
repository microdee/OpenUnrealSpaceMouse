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



#pragma once

#include "CoreMinimal.h"

class SWidget;
class SOverlay;
class SViewport;
class STextBlock;
class FEditorViewportClient;

namespace SpaceMouse::Editor
{
	class SPACEMOUSEEDITOR_API FSmViewportOverlay
	{
	public:
		// TODO: Pray to your god that it ain't gonna dangle
		FSmViewportOverlay(FEditorViewportClient* vpClient);
		~FSmViewportOverlay();

		void Draw(FVector pivot, float pivotDistance) const;
	
	private:
		FEditorViewportClient* ViewportClient = nullptr;
		TSharedPtr<STextBlock> DistanceText;
		TSharedPtr<SViewport> Viewport;
		TSharedPtr<SOverlay> Overlay;

		static TSharedPtr<SWidget> GetChildWidgetOfType(TSharedPtr<SWidget> const& widget, FName const& type);
	};
}
