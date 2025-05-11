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
/**
 * Object managing the lifespan of orbiting information over the active viewport
 */
class SPACEMOUSEEDITOR_API FSmViewportOverlay
{
public:
    FSmViewportOverlay(FEditorViewportClient* VpClient);
    ~FSmViewportOverlay();

    void Draw(FVector Pivot, float PivotDistance);
    
private:
    FEditorViewportClient* ViewportClient = nullptr;
    TSharedPtr<STextBlock> DistanceText;
    TSharedPtr<SViewport> Viewport;
    TSharedPtr<SOverlay> Overlay;

    static TSharedPtr<SWidget> GetChildWidgetOfType(TSharedPtr<SWidget> InWidget, FName InType);
};
