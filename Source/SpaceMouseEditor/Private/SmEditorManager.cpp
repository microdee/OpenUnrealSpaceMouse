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
//#include "Object.h"

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
}

void FSmEditorManager::Tick(float DeltaSecs)
{
    auto Settings = GetMutableDefault<USpaceMouseConfig>();
    FSpaceMouseManager::Tick(DeltaSecs);

    // TODO: ignore camera movement when the player possesses a Pawn in PIE, but not when ejected or only SIE

    ManageActiveViewport();
    ManageOrbitingOverlay();
    TriggerCustomButtons();
    MoveActiveViewport(GetTranslation(), GetRotation());
    
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
