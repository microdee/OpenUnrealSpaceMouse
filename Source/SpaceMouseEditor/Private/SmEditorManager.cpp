// Copyright 2018-2021 David Morasz All Rights Reserved.
// This source code is under MIT License https://github.com/microdee/UE4-SpaceMouse/blob/master/LICENSE

#include "SmEditorManager.h"
#include "SmViewportOverlay.h"

#include "CameraController.h"
#include "CommonBehaviors.h"
#include "SpaceMouseEditor.h"
#include "SmInputDevice.h"
#include "Editor.h"
#include "SEditorViewport.h"
#include "EditorViewportClient.h"
#include "MovementState.h"
#include "HAL/PlatformApplicationMisc.h"
#include "SmUeVersion.h"
#include "Framework/Application/SlateApplication.h"
#include "SGraphPanel.h"

bool FSmMouseWheelInputProcessor::HandleMouseWheelOrGestureEvent(FSlateApplication& SlateApp, const FPointerEvent& InWheelEvent, const FPointerEvent* InGestureEvent)
{
    if (!Owner)
    {
        return false;
    }
    
    float WheelDelta = InWheelEvent.GetWheelDelta();
    if (FMath::IsNearlyZero(WheelDelta))
    {
        return false;
    }
    
    bool bCtrlDown = InWheelEvent.IsControlDown();
    return Owner->ApplyMouseWheelZoom(WheelDelta, InWheelEvent.GetScreenSpacePosition(), bCtrlDown);
}

float FFractionalZoomLevelsContainer::GetDiscreteZoomLevel(int32 Level)
{
    Level = FMath::Clamp(Level, 0, NumZoomLevels - 1);
    return DiscreteZoomLevels[Level];
}

int32 FFractionalZoomLevelsContainer::GetNearestDiscreteLevel(float ZoomAmount)
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

float FFractionalZoomLevelsContainer::GetZoomAmount(int32 InZoomLevel) const
{
    return CurrentZoom;
}

int32 FFractionalZoomLevelsContainer::GetNearestZoomLevel(float InZoomAmount) const
{
    const_cast<FFractionalZoomLevelsContainer*>(this)->CurrentZoom = FMath::Clamp(InZoomAmount, MinZoom, MaxZoom);
    return 0;
}

FText FFractionalZoomLevelsContainer::GetZoomText(int32 InZoomLevel) const
{
    return FText::Format(
        NSLOCTEXT("GraphEditor", "ZoomPercent", "Zoom {0}%"),
        FText::AsNumber(FMath::RoundToInt(CurrentZoom * 100.0f))
    );
}

// From EGraphRenderingLOD::Type
EGraphRenderingLOD::Type FFractionalZoomLevelsContainer::GetLOD(int32 InZoomLevel) const
{
    if (CurrentZoom <= 0.20f) return EGraphRenderingLOD::LowestDetail;
    if (CurrentZoom <= 0.25f) return EGraphRenderingLOD::LowDetail;
    if (CurrentZoom <= 0.675f) return EGraphRenderingLOD::MediumDetail;
    if (CurrentZoom <= 1.375f) return EGraphRenderingLOD::DefaultDetail;
    return EGraphRenderingLOD::FullyZoomedIn;
}

void FSmEditorManager::Initialize()
{
    FSpaceMouseManager::Initialize();

    OnTickDel = OnTickDel.CreateLambda([this]()
    {
        Tick(FApp::GetDeltaTime());
    });
    bWasOrbitCamera = false;
    bWasRealtime = false;
    
    auto& Ibm = FInputBindingManager::Get();
    Ibm.SaveInputBindings();
    
    RegisterMouseWheelProcessor();
}

void FSmEditorManager::Tick(float DeltaSecs)
{
    auto Settings = GetMutableDefault<USpaceMouseConfig>();
    FSpaceMouseManager::Tick(DeltaSecs);


    // Check for Blueprint graph focus first
    ManageActiveBlueprintGraph();
    
    if (ActiveGraphPanel.IsValid())
    {
        // Blueprint graph is focused - handle it (mouse wheel handled via input processor)
        MoveBlueprintGraph(GetTranslation(), GetRotation());
    }
    else
    {
        // TODO: ignore camera movement when the player possesses a Pawn in PIE, but not when ejected or only SIE

        // Normal 3D viewport handling
        ManageActiveViewport();
        ManageOrbitingOverlay();
        TriggerCustomButtons();
        MoveActiveViewport(GetTranslation(), GetRotation());
    }
    
    if(bFinishLearning)
    {
        bLearning = bFinishLearning = false;
    }
    
    if(Enabled) GEditor->GetTimerManager().Get().SetTimerForNextTick(OnTickDel);
}

bool FSmEditorManager::bStarted = false;

void FSmEditorManager::Start()
{
    if (!bStarted)
    {
        bStarted = true;
        GEditor->GetTimerManager().Get().SetTimerForNextTick(OnTickDel);
    }
}

void FSmEditorManager::ManageOrbitingOverlay()
{
    auto Settings = GetMutableDefault<USpaceMouseConfig>();
    if(MovementState->bOnMovementStartedFrame
        && ActiveViewportClient
        && Settings->DisplayOrbitingPivot)
    {
        OrbitingOverlay = MakeShared<FSmViewportOverlay>(ActiveViewportClient);
    }
    if(MovementState->bOnMovementEndedFrame)
    {
        OrbitingOverlay.Reset();
    }
}

void FSmEditorManager::ManageActiveViewport()
{
#if UE_VERSION >= MAKE_UE_VERSION(4, 22)
    TArray<FEditorViewportClient*> AllViewportClients = GEditor->GetAllViewportClients();
#else
    TArray<FEditorViewportClient*> AllViewportClients = GEditor->AllViewportClients;
#endif

    if (IsActiveViewportInvalid(AllViewportClients)) ActiveViewportClient = nullptr;

    for (FEditorViewportClient* Cvp : AllViewportClients)
    {
        if (!Cvp) continue;
        if (!Cvp->GetEditorViewportWidget().Get()) continue;
        if (Cvp->GetEditorViewportWidget().Get()->HasAnyUserFocusOrFocusedDescendants())
        {
            if(Cvp == ActiveViewportClient) break;
            if (Cvp->IsVisible() /* && Cvp->IsPerspective() */)
            {
                if (Cvp != ActiveViewportClient)
                {
                    if (ActiveViewportClient)
                    {
                        ActiveViewportClient->ToggleOrbitCamera(bWasOrbitCamera);
                        ActiveViewportClient->SetRealtime(bWasRealtime);
                    }
                    bWasOrbitCamera = Cvp->ShouldOrbitCamera();
                    bWasRealtime = Cvp->IsRealtime();
                    //cvp->ToggleOrbitCamera(false);
                    //cvp->SetRealtime(true);
                    ActiveViewportClient = Cvp;
                    break;
                }
            }
        }
    }
}

void FSmEditorManager::TriggerCustomButtons()
{
    if (!ActiveViewportClient || !Enabled) return;
    if (!ActiveViewportClient->IsVisible()) return;
    
    auto Settings = GetMutableDefault<USpaceMouseConfig>();
    if(!Settings->ActiveInBackground)
    {
        if(!FPlatformApplicationMisc::IsThisApplicationForeground()) return;
    }
    
    float camspeed = ActiveViewportClient->GetCameraSpeedSetting();
    
    if(!bLearning)
    {
        if (ButtonDownFrame(FSmInputDevice::GetButtonFrom(Settings->DecreaseSpeedButton)))
        {
            ActiveViewportClient->SetCameraSpeedSetting(camspeed - 1);
        }
        if (ButtonDownFrame(FSmInputDevice::GetButtonFrom(Settings->IncreaseSpeedButton)))
        {
            ActiveViewportClient->SetCameraSpeedSetting(camspeed + 1);
        }
        if (ButtonDownFrame(FSmInputDevice::GetButtonFrom(Settings->ResetSpeedButton)))
        {
            ActiveViewportClient->SetCameraSpeedSetting(4);
        }
        if (ButtonDownFrame(FSmInputDevice::GetButtonFrom(Settings->ResetRollButton)))
        {
            ActiveViewportClient->RemoveCameraRoll();
        }
        if (ButtonDownFrame(FSmInputDevice::GetButtonFrom(Settings->ShowSpaceMousePreferencesButton)))
        {
            Settings->GoToSmConfig();
        }
        if (ButtonDownFrame(FSmInputDevice::GetButtonFrom(Settings->ShowInputBindingsButton)))
        {
            Settings->GoToInputBindings();
        }
                
        // Editor actions have been off-loaded to Keyboard Shortcuts in Editor Preferences
    }
}

FVector FSmEditorManager::GetOrbitingPosDeltaOffset(FRotator rotDelta, float forwardDelta)
{
    auto Settings = GetMutableDefault<USpaceMouseConfig>();
    if(MovementState->bOnMovementStartedFrame)
    {
        auto world = ActiveViewportClient->GetWorld();
        FHitResult hit;
        float traceLength = Settings->OrbitingLineTraceLength;
        FVector startpoint = ActiveViewportClient->GetViewLocation();
        FVector endpoint = startpoint +
            ActiveViewportClient->GetViewRotation().RotateVector({1, 0, 0}) * traceLength;

        FCollisionQueryParams ColQuery;
        ColQuery.bTraceComplex = true;

        if(world->LineTraceSingleByChannel(hit, startpoint, endpoint, ECC_Visibility, ColQuery))
        {
            LastOrbitPivot = hit.ImpactPoint;
            LastOrbitDistance = hit.Distance;

            LastOrbitPivotView = LastOrbitPivot - ActiveViewportClient->GetViewLocation();
            LastOrbitPivotView = ActiveViewportClient->GetViewRotation().GetInverse().RotateVector(LastOrbitPivotView);
        }
        if(FMath::IsNearlyZero(LastOrbitDistance))
        {
            LastOrbitDistance = 300;
            LastOrbitPivotView = {LastOrbitDistance, 0, 0};

            LastOrbitPivot = ActiveViewportClient->GetViewLocation() +
                ActiveViewportClient->GetViewRotation().RotateVector(LastOrbitPivotView);
        }
    }
    
    LastOrbitDistance -= forwardDelta;
    LastOrbitPivotView.X -= forwardDelta;

    if(OrbitingOverlay) OrbitingOverlay->Draw(LastOrbitPivot, LastOrbitDistance);

    return USmCommonBehaviors::GetOrbitingTranslationDelta(
        LastOrbitPivotView,
        ActiveViewportClient->GetViewRotation(),
        rotDelta,
        LastOrbitDistance,
        Settings->CameraBehavior == ESpaceMouseCameraBehavior::OrbitingWithRoll
    );
}

FSmUserSettings FSmEditorManager::GetUserSettings()
{
    return GetMutableDefault<USpaceMouseConfig>()->GetUserSettings();
}

void FSmEditorManager::BeginLearning()
{
    bLearning = true;
    bFinishLearning = false;
}

void FSmEditorManager::EndLearning()
{
    bFinishLearning = true;
}

FKeyEvent FSmEditorManager::GetKeyEventFromKey(const FInputActionKeyMapping& mapping)
{
    const uint32* kc;
    const uint32* cc;
    FInputKeyManager::Get().GetCodesFromKey(mapping.Key, kc, cc);
    
    return FKeyEvent(
        mapping.Key, FModifierKeysState(
            mapping.bShift, false,
            mapping.bCtrl, false,
            mapping.bAlt, false,
            mapping.bCmd, false,
            false
        ),
        0, false, cc ? *cc : 0, kc ? *kc : 0
    );
}

bool FSmEditorManager::AllowPerspectiveCameraMoveEvent(FEditorViewportClient* cvp)
{
    static TSet<FName> IncompatibleViewports =
    {
        FName("SStaticMeshEditorViewport")
    };

    FName widgetType = cvp->GetEditorViewportWidget()->GetType();
    return !IncompatibleViewports.Contains(widgetType);
}

void FSmEditorManager::MoveActiveViewport(FVector trans, FRotator rot)
{
    if (!ActiveViewportClient || !Enabled) return;
    if (!ActiveViewportClient->IsVisible()) return;
    
    auto Settings = GetMutableDefault<USpaceMouseConfig>();
    if(!Settings->ActiveInBackground)
    {
        if(!FPlatformApplicationMisc::IsThisApplicationForeground()) return;
    }
    
    if (MovementState->bOnMovementStartedFrame && ActiveViewportClient)
    {
        bWasRealtime = ActiveViewportClient->IsRealtime();
        bWasOrbitCamera = ActiveViewportClient->ShouldOrbitCamera();
        ActiveViewportClient->ToggleOrbitCamera(false);
        ActiveViewportClient->SetRealtime(true);
    }

    if (MovementState->bOnMovementEndedFrame && ActiveViewportClient)
    {
        ActiveViewportClient->SetRealtime(bWasRealtime);
        //ActiveViewportClient->ToggleOrbitCamera(bWasOrbitCamera);
    }
    
    if (trans.IsNearlyZero(SMALL_NUMBER) && rot.IsNearlyZero(SMALL_NUMBER))
    {
        return;
    }
    
    float speedexp = FMath::Max(ActiveViewportClient->GetCameraSpeedSetting() - 8, 0);
    speedexp += FMath::Min(ActiveViewportClient->GetCameraSpeedSetting(), 0);
    float speedmul = FMath::Pow(2, speedexp);
    speedmul *= ActiveViewportClient->GetCameraSpeed();

    bool orbitMovesObject = Settings->OrbitingMovesObject;
    bool orbitRotatesObject = Settings->OrbitingRotatesObject;
    
    if (ActiveViewportClient->IsPerspective())
    {
        FRotator currRot = ActiveViewportClient->GetViewRotation();

        if (Settings->CameraBehavior >= ESpaceMouseCameraBehavior::OrbitingWithRoll)
        {
            auto OrbRot = rot;
            if (Settings->CameraBehavior == ESpaceMouseCameraBehavior::OrbitingNoRoll)
            {
                OrbRot.Pitch *= currRot.Pitch > -80 && currRot.Pitch < 80;
            }
            auto orbitTrans = orbitMovesObject ? -trans : trans;
            trans = GetOrbitingPosDeltaOffset(
                orbitRotatesObject ? OrbRot : OrbRot.GetInverse(),
                orbitTrans.X * speedmul
            ) / speedmul + orbitTrans;
        }
        
        FVector posDelta = currRot.RotateVector(trans) * speedmul;

        FVector currPos = ActiveViewportClient->GetViewLocation();
        currPos += posDelta;
        switch (Settings->CameraBehavior)
        {
        case ESpaceMouseCameraBehavior::CameraDeltaWithRoll:
            currRot = FRotator(FQuat(currRot) * FQuat(rot));
            break;
            
        case ESpaceMouseCameraBehavior::CameraDeltaNoRoll:
            if (MovementState->bOnMovementStartedFrame)
            {
                ActiveViewportClient->RemoveCameraRoll();
            }
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
            if (MovementState->bOnMovementStartedFrame)
            {
                ActiveViewportClient->RemoveCameraRoll();
            }
            currRot.Pitch = FMath::Clamp(currRot.Pitch, -80.0f, 80.0f);
            currRot = FRotator(
                FQuat(FRotator(0, orbitRotatesObject ? -rot.Yaw : rot.Yaw, 0)) *
                FQuat(currRot) *
                FQuat(FRotator(orbitRotatesObject ? -rot.Pitch : rot.Pitch, 0, 0))
            );
            currRot.Roll = 0;
            break;
        default: ;
        }
        ActiveViewportClient->SetViewLocation(currPos);
        ActiveViewportClient->SetViewRotation(currRot);
    }
    else if(ActiveViewportClient->IsOrtho())
    {
        float currZoom = ActiveViewportClient->GetOrthoZoom();
        float currZoomSpeed = currZoom / Settings->TranslationUnitsPerSec * 0.25;
        float zoomDelta = Settings->OrthoPanningPlane == EOrthoSmPlane::LateralIsZoomVerticalIsUp
            ? trans.X : -trans.Z;
        
        zoomDelta *= speedmul * currZoomSpeed * Settings->OrthoZoomSpeed * 8;
        if(Settings->OrbitingMovesObject) zoomDelta *= -1;
        currZoom -= zoomDelta;
        
        if(currZoom < 1) currZoom = 1;
        
        FVector currPos = ActiveViewportClient->ViewTransformOrthographic.GetLocation();
        FRotator currRot;
        switch (ActiveViewportClient->GetViewportType())
        {
        case LVT_OrthoXY:
            {
                currRot = FRotationMatrix::MakeFromX({0, 0, -1}).Rotator();
                currRot = FRotator(currRot.Quaternion() * FRotator(0, 0, -90).Quaternion());
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
                currRot = FRotator(currRot.Quaternion() * FRotator(0, 0, 90).Quaternion());
                break;
            }
        case LVT_OrthoNegativeXZ: currRot = FRotationMatrix::MakeFromX({0, 1, 0}).Rotator(); break;
        case LVT_OrthoNegativeYZ: currRot = FRotationMatrix::MakeFromX({-1, 0, 0}).Rotator(); break;
        default: ;
        }
        
        FVector orthoTrans;

        switch (Settings->OrthoPanningPlane)
        {
        case EOrthoSmPlane::LateralIsZoomVerticalIsUp: orthoTrans = {0, trans.Y, trans.Z}; break;
        case EOrthoSmPlane::LateralIsUpVerticalIsZoom: orthoTrans = {0, trans.Y, trans.X}; break;
        }
        
        FVector posDelta = currRot.RotateVector(orthoTrans) * speedmul * currZoomSpeed;
        if(Settings->OrbitingMovesObject) posDelta *= -1;
        currPos += posDelta;

        ActiveViewportClient->ViewTransformOrthographic.SetLocation(currPos);
        ActiveViewportClient->SetOrthoZoom(currZoom);
    }
    if(AllowPerspectiveCameraMoveEvent(ActiveViewportClient))
    {
        // This is important to trigger PerspectiveCameraMoved event from outside.
        ActiveViewportClient->MoveViewportCamera(FVector::ZeroVector, FRotator::ZeroRotator);
    }
    ActiveViewportClient->Viewport->InvalidateHitProxy();
}

const bool FSmEditorManager::IsActiveViewportInvalid(const TArray<FEditorViewportClient*>& AllViewportClients)
{
    bool bActiveViewportInvalid = true;
    for (FEditorViewportClient* Cvp : AllViewportClients)
    {
        if (Cvp == ActiveViewportClient) return false;
    }
    return true;
}

void FSmEditorManager::ManageActiveBlueprintGraph()
{
    auto Settings = GetMutableDefault<USpaceMouseConfig>();
    
    if (!Settings->bEnableBlueprintNavigation)
    {
        ActiveGraphPanel.Reset();
        BlueprintZoomAccumulator = 0.0f;
        bInstalledFractionalZoom = false;
        CurrentDiscreteZoomLevel = 11;
        return;
    }
    
    TSharedPtr<SGraphPanel> FoundPanel = FindFocusedGraphPanel();
    
    if (FoundPanel.IsValid())
    {
        // Reset accumulator when switching to a different panel
        if (ActiveGraphPanel.Pin() != FoundPanel)
        {
            BlueprintZoomAccumulator = 0.0f;
            bInstalledFractionalZoom = false;
            // Initialize discrete level from current zoom
            float CurrentZoom = FoundPanel->GetZoomAmount();
            CurrentDiscreteZoomLevel = FFractionalZoomLevelsContainer::GetNearestDiscreteLevel(CurrentZoom);
        }
        ActiveGraphPanel = FoundPanel;
        
        // Install fractional zoom if not already done
        if (!bInstalledFractionalZoom)
        {
            InstallFractionalZoom(FoundPanel);
        }
    }
    else
    {
        ActiveGraphPanel.Reset();
        BlueprintZoomAccumulator = 0.0f;
        bInstalledFractionalZoom = false;
        CurrentDiscreteZoomLevel = 11;
    }
}

void FSmEditorManager::InstallFractionalZoom(TSharedPtr<SGraphPanel> GraphPanel)
{
    if (!GraphPanel.IsValid())
    {
        return;
    }
    
    // Get the current zoom before replacing the container
    float CurrentZoom = GraphPanel->GetZoomAmount();
    
    // Initialize discrete level from current zoom
    CurrentDiscreteZoomLevel = FFractionalZoomLevelsContainer::GetNearestDiscreteLevel(CurrentZoom);
    
    // Install a custom fractional zoom container
    GraphPanel->SetZoomLevelsContainer<FFractionalZoomLevelsContainer>();
    
    // Restore the zoom level
    FVector2D CurrentOffset = GraphPanel->GetViewOffset();
    GraphPanel->RestoreViewSettings(CurrentOffset, CurrentZoom, FGuid());
    
    bInstalledFractionalZoom = true;
}

void FSmEditorManager::RegisterMouseWheelProcessor()
{
    if (!FSlateApplication::IsInitialized())
    {
        return;
    }
    
    if (!MouseWheelProcessor.IsValid())
    {
        MouseWheelProcessor = MakeShared<FSmMouseWheelInputProcessor>(this);
        FSlateApplication::Get().RegisterInputPreProcessor(MouseWheelProcessor);
    }
}

void FSmEditorManager::UnregisterMouseWheelProcessor()
{
    if (FSlateApplication::IsInitialized() && MouseWheelProcessor.IsValid())
    {
        FSlateApplication::Get().UnregisterInputPreProcessor(MouseWheelProcessor);
        MouseWheelProcessor.Reset();
    }
}

bool FSmEditorManager::ApplyMouseWheelZoom(float WheelDelta, FVector2D ScreenSpacePosition, bool bCtrlDown)
{
    auto Settings = GetMutableDefault<USpaceMouseConfig>();
    if (!Settings->bEnableBlueprintNavigation)
    {
        return false;
    }
    
    if (!Settings->ActiveInBackground &&
        !FPlatformApplicationMisc::IsThisApplicationForeground())
        {
            return false;
    }
    
    if (!FSlateApplication::IsInitialized())
    {
        return false;
    }
    
    FSlateApplication& SlateApp = FSlateApplication::Get();
    FWidgetPath WidgetPath = SlateApp.LocateWindowUnderMouse(ScreenSpacePosition, SlateApp.GetInteractiveTopLevelWindows());
    
    TSharedPtr<SGraphPanel> GraphPanel;
    for (int32 i = WidgetPath.Widgets.Num() - 1; i >= 0; --i)
    {
        TSharedRef<SWidget> Widget = WidgetPath.Widgets[i].Widget;
        if (Widget->GetType() == FName("SGraphPanel"))
        {
            GraphPanel = StaticCastSharedRef<SGraphPanel>(Widget);
            break;
        }
    }
    
    if (!GraphPanel.IsValid())
    {
        return false;
    }
    
    // Only handle if we've already installed fractional zoom on this panel (from ManageActiveBlueprintGraph)
    if (ActiveGraphPanel.Pin() != GraphPanel || !bInstalledFractionalZoom)
    {
        // Let default Slate handling occur - we haven't taken over this panel yet
        return false;
    }
    
    // Get current zoom state
    float CurrentZoom = GraphPanel->GetZoomAmount();
    FVector2D CurrentOffset = GraphPanel->GetViewOffset();
    
    int32 OldLevel = CurrentDiscreteZoomLevel;
    
    // Index 11 is 100% - require Ctrl to zoom past it
    constexpr int32 ZoomLevel100Percent = 11;
    
    // Apply discrete zoom step based on wheel direction
    if (WheelDelta > 0)
    {
        // Scroll up = zoom in = higher level
        int32 MaxLevel = bCtrlDown ? (FFractionalZoomLevelsContainer::NumZoomLevels - 1) : ZoomLevel100Percent;
        CurrentDiscreteZoomLevel = FMath::Min(CurrentDiscreteZoomLevel + 1, MaxLevel);
    }
    else
    {
        // Scroll down = zoom out = lower level
        CurrentDiscreteZoomLevel = FMath::Max(CurrentDiscreteZoomLevel - 1, 0);
    }
    
    // Get the target zoom for this discrete level
    float NewZoom = FFractionalZoomLevelsContainer::GetDiscreteZoomLevel(CurrentDiscreteZoomLevel);
    
    UE_LOG(LogTemp, Log, TEXT("MouseWheel Zoom: Level %d -> %d, Zoom %.3f -> %.3f"), 
        OldLevel, CurrentDiscreteZoomLevel, CurrentZoom, NewZoom);
    
    // Get local mouse position for zoom-toward-cursor
    FGeometry PanelGeometry = GraphPanel->GetCachedGeometry();
    FVector2D LocalMousePos = PanelGeometry.AbsoluteToLocal(ScreenSpacePosition);
    
    // Convert zoom point to graph space at current zoom
    FVector2D PointInGraph = CurrentOffset + (LocalMousePos / CurrentZoom);
    
    // Adjust offset so point stays at same graph position after zoom
    FVector2D NewOffset = PointInGraph - (LocalMousePos / NewZoom);
    
    GraphPanel->RestoreViewSettings(NewOffset, NewZoom, FGuid());
    
    // We handled this event - consume it so Slate doesn't process it too
    return true;
}

TSharedPtr<SGraphPanel> FSmEditorManager::FindFocusedGraphPanel()
{
    if (!FSlateApplication::IsInitialized())
    {
        return nullptr;
    }
    
    FSlateApplication& SlateApp = FSlateApplication::Get();
    TSharedPtr<SWidget> FocusedWidget = SlateApp.GetUserFocusedWidget(0);
    
    if (!FocusedWidget.IsValid())
    {
        return nullptr;
    }
    
    // Check if focused widget is a graph panel
    if (FocusedWidget->GetType() == FName("SGraphPanel"))
    {
        return StaticCastSharedPtr<SGraphPanel>(FocusedWidget);
    }
    
    // Search ancestors for graph panel
    TSharedPtr<SWidget> CurrentWidget = FocusedWidget;
    while (CurrentWidget.IsValid())
    {
        TSharedPtr<SWidget> ParentWidget = CurrentWidget->GetParentWidget();
        if (!ParentWidget.IsValid())
        {
            break;
        }
        
        if (ParentWidget->GetType() == FName("SGraphPanel"))
        {
            return StaticCastSharedPtr<SGraphPanel>(ParentWidget);
        }
        
        CurrentWidget = ParentWidget;
    }
    
    return nullptr;
}

TSharedPtr<SGraphPanel> FSmEditorManager::FindGraphPanelInWidget(TSharedPtr<SWidget> Widget)
{
    if (!Widget.IsValid())
    {
        return nullptr;
    }
    
    if (Widget->GetType() == FName("SGraphPanel"))
    {
        return StaticCastSharedPtr<SGraphPanel>(Widget);
    }
    
    FChildren* Children = Widget->GetChildren();
    if (Children)
    {
        for (int32 i = 0; i < Children->Num(); ++i)
        {
            TSharedRef<SWidget> Child = Children->GetChildAt(i);
            TSharedPtr<SGraphPanel> Found = FindGraphPanelInWidget(Child);
            if (Found.IsValid())
            {
                return Found;
            }
        }
    }
    
    return nullptr;
}

void FSmEditorManager::MoveBlueprintGraph(FVector trans, FRotator rot)
{
    if (!ActiveGraphPanel.IsValid() || !Enabled)
    {
        return;
    }
    
    auto Settings = GetMutableDefault<USpaceMouseConfig>();
    
    if (!Settings->ActiveInBackground &&
        !FPlatformApplicationMisc::IsThisApplicationForeground())
    {
        return;
    }
    
    if (trans.IsNearlyZero(SMALL_NUMBER) && rot.IsNearlyZero(SMALL_NUMBER))
    {
        return;
    }
    
    TSharedPtr<SGraphPanel> GraphPanel = ActiveGraphPanel.Pin();
    if (!GraphPanel.IsValid())
    {
        return;
    }
    
    // Get current view state
    FVector2D CurrentOffset = GraphPanel->GetViewOffset();
    float CurrentZoom = GraphPanel->GetZoomAmount();
    
    // Orientation mapping:
    // - trans.Y = left/right → Blueprint X
    // - trans.X = up/down    → Blueprint Y
    // - trans.Z = zoom       → Blueprint zoom
    float PanX = trans.Y * Settings->BlueprintPanSpeed * (2.5f);
    float PanY = trans.X * Settings->BlueprintPanSpeed * (2.5f);
    
    if (Settings->bBlueprintInvertPanX) PanX = -PanX;
    if (Settings->bBlueprintInvertPanY) PanY = -PanY;
    
    // Apply pan (divide by zoom to make panning consistent at all zoom levels)
    FVector2D NewOffset = CurrentOffset;
    NewOffset.X -= PanX / CurrentZoom;
    NewOffset.Y += PanY / CurrentZoom;
    
    // SMOOTH FRACTIONAL ZOOM - no more stepping!
    // With our custom FFractionalZoomLevelsContainer installed, we can set any zoom value
    float ZoomInput = trans.Z * Settings->BlueprintZoomSpeed * 0.005f;
    if (Settings->bBlueprintInvertZoom) ZoomInput = -ZoomInput;
    
    // Apply zoom as a multiplier for smooth scaling
    float ZoomMultiplier = 1.0f + ZoomInput;
    float NewZoom = CurrentZoom * ZoomMultiplier;
    
    // Clamp to limits
    const float MinZoom = 0.1f;
    const float MaxZoom = 2.0f;
    NewZoom = FMath::Clamp(NewZoom, MinZoom, MaxZoom);
    
    bool bZoomChanged = !FMath::IsNearlyEqual(NewZoom, CurrentZoom, 0.0001f);
    
    // Zoom toward center of the panel
    if (bZoomChanged && NewZoom > 0.0f)
    {
        FGeometry PanelGeometry = GraphPanel->GetCachedGeometry();
        FVector2D PanelSize = FVector2D(PanelGeometry.GetLocalSize());
        FVector2D PanelCenter = PanelSize * 0.5;
        
        // Convert center to graph space at current zoom
        FVector2D CenterInGraph = NewOffset + (PanelCenter / CurrentZoom);
        
        // Adjust offset so center stays at same graph position after zoom change
        NewOffset = CenterInGraph - (PanelCenter / NewZoom);
    }
    
    // Apply the view settings - with our fractional zoom container, this is smooth!
    bool bPanChanged = !NewOffset.Equals(CurrentOffset, 0.01f);
    if ((bPanChanged || bZoomChanged) && NewZoom > 0.0f)
    {
        GraphPanel->RestoreViewSettings(NewOffset, NewZoom, FGuid());
    }
}