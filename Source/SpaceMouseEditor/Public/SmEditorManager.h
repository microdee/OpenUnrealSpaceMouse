// Copyright 2018-2021 David Morasz All Rights Reserved.
// This source code is under MIT License https://github.com/microdee/UE4-SpaceMouse/blob/master/LICENSE

#pragma once

#include "SpaceMouseManager.h"
#include "Misc/App.h"
#include "Input/Events.h"
#include "TimerManager.h"
#include "SNodePanel.h"
#include "Framework/Application/IInputProcessor.h"

struct FInputActionKeyMapping;
class FEditorViewportClient;
class FSmViewportOverlay;
class SGraphPanel;
class SWidget;
class FSmEditorManager;

/**
 * Input preprocessor that intercepts mouse wheel events for blueprint graph zoom.
 */
class SPACEMOUSEEDITOR_API FSmMouseWheelInputProcessor : public IInputProcessor
{
public:
    FSmMouseWheelInputProcessor(FSmEditorManager* InOwner) : Owner(InOwner) {}
    
    virtual void Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor) override {}
    virtual bool HandleMouseWheelOrGestureEvent(FSlateApplication& SlateApp, const FPointerEvent& InWheelEvent, const FPointerEvent* InGestureEvent) override;
    
private:
    FSmEditorManager* Owner = nullptr;
};

/**
 * Custom zoom levels container that allows truly fractional/continuous zooming.
 * Instead of fixed discrete levels, this stores the current zoom as a float
 * and returns it directly, allowing smooth zooming.
 */
struct SPACEMOUSEEDITOR_API FFractionalZoomLevelsContainer : public FZoomLevelsContainer
{
    float CurrentZoom = 1.0f;
    
    static constexpr float MinZoom = 0.1f;
    static constexpr float MaxZoom = 2.0f;
    static constexpr int32 NumZoomLevels = 17;
    
    /** Fixed discrete zoom levels for mouse wheel */
    static constexpr float DiscreteZoomLevels[NumZoomLevels] = {
        0.10f,
        0.15f,
        0.20f,
        0.25f,
        0.33f,
        0.50f,
        0.60f,
        0.69f, // nice
        0.75f,
        0.80f,
        0.90f,
        1.00f,
        1.10f,
        1.25f,
        1.50f,
        1.75f,
        2.00f 
    };
    
    FFractionalZoomLevelsContainer() : CurrentZoom(1.0f) {}
    
    /**
     * Get the zoom value for a discrete zoom level index.
     */
    static float GetDiscreteZoomLevel(int32 Level)
    {
        Level = FMath::Clamp(Level, 0, NumZoomLevels - 1);
        return DiscreteZoomLevels[Level];
    }
    
    /**
     * Get the nearest discrete zoom level index for a given zoom amount.
     */
    static int32 GetNearestDiscreteLevel(float ZoomAmount)
    {
        ZoomAmount = FMath::Clamp(ZoomAmount, MinZoom, MaxZoom);
        int32 NearestLevel = 0;
        float SmallestDiff = FMath::Abs(ZoomAmount - DiscreteZoomLevels[0]);
        for (int32 i = 1; i < NumZoomLevels; ++i)
        {
            float Diff = FMath::Abs(ZoomAmount - DiscreteZoomLevels[i]);
            if (Diff < SmallestDiff)
            {
                SmallestDiff = Diff;
                NearestLevel = i;
            }
        }
        return NearestLevel;
    }
    
    void SetZoom(float InZoom)
    {
        CurrentZoom = FMath::Clamp(InZoom, MinZoom, MaxZoom);
    }
    
    virtual float GetZoomAmount(int32 InZoomLevel) const override
    {
        // Ignore the index, just return our current zoom
        return CurrentZoom;
    }
    
    virtual int32 GetNearestZoomLevel(float InZoomAmount) const override
    {
        // Store the requested zoom and return 0 (we only have one "level")
        const_cast<FFractionalZoomLevelsContainer*>(this)->CurrentZoom = FMath::Clamp(InZoomAmount, MinZoom, MaxZoom);
        return 0;
    }
    
    virtual FText GetZoomText(int32 InZoomLevel) const override
    {
        return FText::Format(NSLOCTEXT("GraphEditor", "ZoomPercent", "Zoom {0}%"), FText::AsNumber(FMath::RoundToInt(CurrentZoom * 100.0f)));
    }
    
    virtual int32 GetNumZoomLevels() const override
    {
        return 1;
    }
    
    virtual int32 GetDefaultZoomLevel() const override
    {
        return 0;
    }
    
    virtual EGraphRenderingLOD::Type GetLOD(int32 InZoomLevel) const override
    {
        if (CurrentZoom <= 0.25f) return EGraphRenderingLOD::LowestDetail;
        if (CurrentZoom <= 0.50f) return EGraphRenderingLOD::LowDetail;
        if (CurrentZoom <= 0.75f) return EGraphRenderingLOD::MediumDetail;
        if (CurrentZoom <= 1.25f) return EGraphRenderingLOD::DefaultDetail;
        return EGraphRenderingLOD::FullyZoomedIn;
    }
};

class SPACEMOUSEEDITOR_API FSmEditorManager : public FSpaceMouseManager
{
private:

    FTimerDelegate OnTickDel;

    bool bLearning = false;
    bool bFinishLearning = false;

    bool bWasOrbitCamera = false;
    bool bWasRealtime = false;
    float LastOrbitDistance = 0;
    FVector LastOrbitPivot = FVector::ZeroVector;
    FVector LastOrbitPivotView = FVector::ZeroVector;

    FEditorViewportClient* ActiveViewportClient = nullptr;
    FString FocusedVpType = "";

    TWeakPtr<SGraphPanel> ActiveGraphPanel;
    float BlueprintZoomAccumulator = 0.0f;
    bool bInstalledFractionalZoom = false;
    int32 CurrentDiscreteZoomLevel = 11;  // Default to 100% (index 11 of 0-16)
    TSharedPtr<FSmMouseWheelInputProcessor> MouseWheelProcessor;

    TSharedPtr<FSmViewportOverlay> OrbitingOverlay;

    static FKeyEvent GetKeyEventFromKey(const FInputActionKeyMapping& mapping);
    static bool AllowPerspectiveCameraMoveEvent(FEditorViewportClient* cvp);
    FVector GetOrbitingPosDeltaOffset(FRotator rotDelta, float forwardDelta);

protected:
    virtual FSmUserSettings GetUserSettings() override;

public:

    static bool bStarted;

    void BeginLearning();
    void EndLearning();
    bool IsLearning() const { return bLearning; } 

    virtual void Initialize() override;
    virtual void Tick(float DeltaSecs) override;
    void Start();
    void ManageOrbitingOverlay();
    void ManageActiveViewport();
    void ManageActiveBlueprintGraph();
    void TriggerCustomButtons();
    void MoveActiveViewport(FVector trans, FRotator rot);
    void MoveBlueprintGraph(FVector trans, FRotator rot);
    const bool IsActiveViewportInvalid(const TArray<FEditorViewportClient*>& AllViewportClients);
    
    /** Apply a discrete zoom step from mouse wheel input. Returns true if handled. */
    bool ApplyMouseWheelZoom(float WheelDelta, FVector2D ScreenSpacePosition, bool bCtrlDown);
    
    /** Get the currently active graph panel (for input processor) */
    TSharedPtr<SGraphPanel> GetActiveGraphPanel() const { return ActiveGraphPanel.Pin(); }

private:
    TSharedPtr<SGraphPanel> FindFocusedGraphPanel();
    TSharedPtr<SGraphPanel> FindGraphPanelInWidget(TSharedPtr<SWidget> Widget);
    void InstallFractionalZoom(TSharedPtr<SGraphPanel> GraphPanel);
    void RegisterMouseWheelProcessor();
    void UnregisterMouseWheelProcessor();
};
