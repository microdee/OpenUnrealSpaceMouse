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
#include "SmKey.h"
#include "SmButtons.h"
#include "UObject/Interface.h"
#include "SmUserSettings.h"
#include "Curves/CurveFloat.h"
#include "IDetailCustomization.h"
#include "GameFramework/PlayerInput.h"

#include "SpaceMouseConfig.generated.h"

class FInputBindingManager;
class FUICommandInfo;

UENUM()
enum class ESpaceMouseCameraBehavior : uint8
{
    CameraDeltaWithRoll UMETA(ToolTip="Puck represents the movement of the camera which can move freely in all 6DoF. Rotation is in camera space."),
    CameraDeltaNoRoll UMETA(ToolTip="Puck represents the movement of the camera which can move freely in 5DoF (without roll). Horizon is kept straight. Pitch is in camera space, Yaw is in world space."),
    OrbitingWithRoll UMETA(ToolTip="Puck can represent either the camera orbiting around a subject, or a subject moving in front of the camera."),
    OrbitingNoRoll UMETA(ToolTip="Puck can represent either the camera orbiting around a subject, or a subject moving in front of the camera. Horizon is kept straight.")
};

UENUM()
enum class EOrthoSmPlane : uint8
{
    LateralIsZoomVerticalIsUp UMETA(ToolTip="Move puck orthogonal to the plane of the Monitor to move around, Push Forward / Pull Backward to zoom."),
    LateralIsUpVerticalIsZoom UMETA(ToolTip="Move puck orthogonal to the plane of the Desk to move around, Push Down / Pull Up to zoom.")
};

USTRUCT(BlueprintType)
struct SPACEMOUSEEDITOR_API FUICommandName
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite, Category = "SpaceMouse")
    FName Context;
    
    UPROPERTY(BlueprintReadWrite, Category = "SpaceMouse")
    FName Command;
};

UCLASS(Config=Editor, defaultconfig)
class SPACEMOUSEEDITOR_API USpaceMouseConfig : public UObject
{
    GENERATED_BODY()
public:
    USpaceMouseConfig(const FObjectInitializer& ObjectInitializer);

    FSmUserSettings GetUserSettings();

    UPROPERTY(EditAnywhere, Config, Category = "Behavior")
    bool ActiveInBackground = false;

    UPROPERTY(EditAnywhere, Config, Category = "Behavior")
    ESpaceMouseCameraBehavior CameraBehavior = ESpaceMouseCameraBehavior::CameraDeltaWithRoll;

    UPROPERTY(EditAnywhere, Config, Category = "Behavior")
    float MovementSecondsTolerance = 0.25;

    UPROPERTY(
        EditAnywhere,
        Config,
        Category = "Behavior|Orbiting",
        meta = (
            ToolTip = "Puck represents the subject moving in front of the viewport while translating"
        )
    )
    bool OrbitingMovesObject = false;

    UPROPERTY(
        EditAnywhere,
        Config,
        Category = "Behavior|Orbiting",
        meta = (
            ToolTip = "Puck represents the subject rotating in front of the viewport while rotating"
        )
    )
    bool OrbitingRotatesObject = false;
    
    UPROPERTY(EditAnywhere, Config, Category = "Behavior|Orbiting")
    bool DisplayOrbitingPivot = true;

    UPROPERTY(
        EditAnywhere,
        Config,
        Category = "Behavior|Orbiting",
        meta = (
            ToolTip = "You cannot orbit around anything larger than this. The default is 2 Km."
        )
    )
    float OrbitingLineTraceLength = 200000; // 2 Km

    UPROPERTY(
        EditAnywhere,
        Config,
        Category = "Behavior|Orthographic Viewports",
        meta = (
            ToolTip = "An arbitrary multiplier of the Zoom speed while insdide an Orthographic viewport. Negative numbers reverse direction."
        )
    )
    float OrthoZoomSpeed = 1.0;

    UPROPERTY(
        EditAnywhere,
        Config,
        Category = "Behavior|Orthographic Viewports",
        meta = (
            ToolTip = "Select the plane of the device (2 axes) which will control the panning motion inside an Orthographic viewport. See tooltips for specific explanations."
        )
    )
    EOrthoSmPlane OrthoPanningPlane = EOrthoSmPlane::LateralIsZoomVerticalIsUp;

    UPROPERTY(EditAnywhere, Config, Category = "Rotation")
    float RotationDegreesPerSec = 270;

    UPROPERTY(EditAnywhere, Config, Category = "Translation")
    float TranslationUnitsPerSec = 1000;

    UPROPERTY(EditAnywhere, Config, Category = "Translation")
    FVector XTranslationAxisMap = FVector(0, -1, 0);

    UPROPERTY(EditAnywhere, Config, Category = "Translation")
    FVector YTranslationAxisMap = FVector(1, 0, 0);

    UPROPERTY(EditAnywhere, Config, Category = "Translation")
    FVector ZTranslationAxisMap = FVector(0, 0, -1);

    UPROPERTY(
        config,
        EditAnywhere,
        Category = "Translation",
        meta = (
            XAxisName = "Push/Pull",
            YAxisName = "Scale"
        )
    )
    FRuntimeFloatCurve TranslationCurve;

    UPROPERTY(EditAnywhere, Config, Category = "Rotation")
    FVector PitchAxisMap = FVector(1, 0, 0);

    UPROPERTY(EditAnywhere, Config, Category = "Rotation")
    FVector YawAxisMap = FVector(0, 0, 1);

    UPROPERTY(EditAnywhere, Config, Category = "Rotation")
    FVector RollAxisMap = FVector(0, -1, 0);

    UPROPERTY(
        config,
        EditAnywhere,
        Category = "Rotation",
        meta = (
            XAxisName = "Push/Pull",
            YAxisName = "Scale"
        )
    )
    FRuntimeFloatCurve RotationCurve;

    UPROPERTY(EditAnywhere, Config, Category = HID)
    int MaxHidReadOperationsPerFrame = 2048;

    UPROPERTY(EditAnywhere, Config, Category = HID)
    bool DisplayDebugInformation = false;

    UPROPERTY(
        EditAnywhere,
        Config,
        Category = "ButtonMapping",
        meta = (
            ToolTip = "Only SpaceMouse buttons regarded here."
        )
    )
    FSmKey IncreaseSpeedButton;

    UPROPERTY(
        EditAnywhere,
        Config,
        Category = "ButtonMapping",
        meta = (
            ToolTip = "Only SpaceMouse buttons regarded here."
        )
    )
    FSmKey DecreaseSpeedButton;

    UPROPERTY(
        EditAnywhere,
        Config,
        Category = "ButtonMapping",
        meta = (
            ToolTip = "Only SpaceMouse buttons regarded here."
        )
    )
    FSmKey ResetSpeedButton;

    UPROPERTY(
        EditAnywhere,
        Config,
        Category = "ButtonMapping",
        meta = (
            ToolTip = "Only SpaceMouse buttons regarded here."
        )
    )
    FSmKey ResetRollButton;

    UPROPERTY(
        EditAnywhere,
        Config,
        Category = "ButtonMapping",
        meta = (
            ToolTip = "Only SpaceMouse buttons regarded here."
        )
    )
    FSmKey ShowSpaceMousePreferencesButton;

    UPROPERTY(
        EditAnywhere,
        Config,
        Category = "ButtonMapping",
        meta = (
            ToolTip = "Only SpaceMouse buttons regarded here."
        )
    )
    FSmKey ShowInputBindingsButton;

    UPROPERTY(
        VisibleAnywhere,
        AdvancedDisplay,
        Config,
        Category = "ButtonMapping",
        meta = (
            ToolTip = "Only SpaceMouse buttons regarded here."
        )
    )
    TMap<FString, FUICommandName> CustomInputBindings;
    
    // Further editor actions have been off-loaded to Keyboard Shortcuts in Editor Preferences

    /**
     * This is used for keeping the bindings from SpaceMouse persistent
     * because of course it doesn't automatically do that for us.
     * This even executes on the next tick because at module initialization it's just too early apparently.
     * This is not exactly a smooth way to do things.
     */
    void RegisterInputBindingNotification();
    
    void GoToSmConfig() const;
    void GoToInputBindings() const;

    static void SetDefaultBindings(bool bAskUser);
    static void SetCommandBinding(FInputBindingManager& Ibm, FName InCmdCtx, FName InCmd, EV3DCmd SmButton);
    static void SetCommandBinding(TSharedPtr<FUICommandInfo> InCmd, EV3DCmd SmButton);

private:
    void RegisterCustomInputBinding(const FUICommandInfo& Cmd);
    void HandleUserDefinedChordChanged(const FUICommandInfo& Cmd);
};

class FSpaceMouseConfigCustomization : public IDetailCustomization
{
public:
    static TSharedRef< IDetailCustomization > MakeInstance();

    virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;
};
