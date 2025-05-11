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




#include "SmInputDevice.h"

#include "MovementState.h"
#include "SmRuntimeManager.h"
#include "SpaceMouseData.h"
#include "SmUeVersion.h"

#define LOCTEXT_NAMESPACE "SpaceMouseRuntime"

TSharedPtr<FSmRuntimeManager> FSmInputDevice::Manager;
TMap<FKey, EV3DCmd> FSmInputDevice::KeyToSmButtonMap;
    
FKey FSmInputDevice::TranslationX;
FKey FSmInputDevice::TranslationY;
FKey FSmInputDevice::TranslationZ;
    
FKey FSmInputDevice::RotationPitch;
FKey FSmInputDevice::RotationYaw;
FKey FSmInputDevice::RotationRoll;

FSmInputDevice::FSmInputDevice(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler)
    : MessageHandler(InMessageHandler)
{
    RegisterSmButtons();
    Manager = MakeShared<FSmRuntimeManager>();
    Manager->Initialize();
}

FSmInputDevice::~FSmInputDevice()
{
}

#define SM_KEY_PREFIX "SpaceMouseUE_"
#define SM_KEY_PREFIX_TEXT TEXT(SM_KEY_PREFIX)

FKey FSmInputDevice::GetKeyFrom(EV3DCmd SmButton)
{
    static FString GlobalPrefix = SM_KEY_PREFIX_TEXT;
    return FKey(*(GlobalPrefix + FSmButton::GetNameOf(SmButton).ToString()));
}

EV3DCmd FSmInputDevice::GetButtonFrom(FKey InKey)
{
    if(KeyToSmButtonMap.Contains(InKey)) return KeyToSmButtonMap[InKey];
    return EV3DCmd::Noop;
}

FKeyDetails FSmInputDevice::GetKeyDetailsFrom(EV3DCmd SmButton)
{
    FString LongDispName = TEXT("SpaceMouse ") + FSmButton::GetFriendlyNameOf(SmButton);
    return FKeyDetails(
        GetKeyFrom(SmButton),
        FText::FromString(LongDispName),
        FKeyDetails::GamepadKey,
        TEXT("SpaceMouse"),
        FText::FromString(FSmButton::GetFriendlyNameOf(SmButton))
    );
}

// Axis flag name has changed in UE 4.26
#if UE_VERSION >= MAKE_UE_VERSION(4, 26)
#define AXIS_FLAG Axis1D
#else
#define AXIS_FLAG FloatAxis
#endif

#define SM_AXIS_DETAIL(InAxis) \
    FKeyDetails( \
        FKey(SM_KEY_PREFIX_TEXT TEXT(InAxis)), \
        LOCTEXT("SmInputDevice_" InAxis, "SpaceMouse " InAxis), \
        FKeyDetails::GamepadKey | FKeyDetails::AXIS_FLAG, \
        TEXT("SpaceMouse") \
    )

void FSmInputDevice::RegisterSmButtons()
{
    // register menu category
    EKeys::AddMenuCategoryDisplayInfo(
        TEXT("SpaceMouse"),
        LOCTEXT("SmInputDevice_InputMenuCategoryName", "SpaceMouse"),
        TEXT("GraphEditor.KeyEvent_16x") // TODO: make our own palette icon?
    );

    // register axes
    auto TrX = SM_AXIS_DETAIL("Lateral");
    auto TrY = SM_AXIS_DETAIL("Horizontal");
    auto TrZ = SM_AXIS_DETAIL("Vertical");
    TranslationX = TrX.GetKey();
    TranslationY = TrY.GetKey();
    TranslationZ = TrZ.GetKey();
    EKeys::AddKey(TrX);
    EKeys::AddKey(TrY);
    EKeys::AddKey(TrZ);
    
    auto RotP = SM_AXIS_DETAIL("Pitch");
    auto RotY = SM_AXIS_DETAIL("Yaw");
    auto RotR = SM_AXIS_DETAIL("Roll");
    RotationPitch = RotP.GetKey();
    RotationYaw = RotY.GetKey();
    RotationRoll = RotR.GetKey();
    EKeys::AddKey(RotP);
    EKeys::AddKey(RotY);
    EKeys::AddKey(RotR);

    // register buttons
    for(auto SmButton : FAllSmButtons())
    {
        auto OutKey = GetKeyDetailsFrom(SmButton);
        EKeys::AddKey(OutKey);
        KeyToSmButtonMap.Add(OutKey.GetKey(), SmButton);
    }
}

void FSmInputDevice::Tick(float DeltaTime)
{
}

void FSmInputDevice::SendControllerEvents()
{
    Manager->Tick(FApp::GetDeltaTime());
    
#if UE_VERSION >= MAKE_UE_VERSION(5, 1)
    
    IPlatformInputDeviceMapper& DeviceMapper = IPlatformInputDeviceMapper::Get();
    FPlatformUserId PlatformUser = PLATFORMUSERID_NONE;
    FInputDeviceId InputDevice = INPUTDEVICEID_NONE;
    DeviceMapper.RemapControllerIdToPlatformUserAndDevice(0, OUT PlatformUser, OUT InputDevice);

    // Send axis data only while moving and an extra frame when axis values are supposedly 0
    if(Manager->MovementState->bMoving || Manager->MovementState->bOnMovementEndedFrame)
    {
        MessageHandler->OnControllerAnalog(
            SM_KEY_PREFIX_TEXT TEXT("Lateral"),
            PlatformUser, InputDevice,
            Manager->GetNormalizedTranslation().X
        );
        MessageHandler->OnControllerAnalog(
            SM_KEY_PREFIX_TEXT TEXT("Horizontal"), 
            PlatformUser, InputDevice,
            Manager->GetNormalizedTranslation().Y
        );
        MessageHandler->OnControllerAnalog(
            SM_KEY_PREFIX_TEXT TEXT("Vertical"), 
            PlatformUser, InputDevice,
            Manager->GetNormalizedTranslation().Z
        );
    
        MessageHandler->OnControllerAnalog(
            SM_KEY_PREFIX_TEXT TEXT("Pitch"), 
            PlatformUser, InputDevice,
            Manager->GetNormalizedRotation().Pitch
        );
        MessageHandler->OnControllerAnalog(
            SM_KEY_PREFIX_TEXT TEXT("Yaw"), 
            PlatformUser, InputDevice,
            Manager->GetNormalizedRotation().Yaw
        );
        MessageHandler->OnControllerAnalog(
            SM_KEY_PREFIX_TEXT TEXT("Roll"), 
            PlatformUser, InputDevice,
            Manager->GetNormalizedRotation().Roll
        );
    }

    for(int i=0; i<Manager->GetButtons().Num(); ++i)
    {
        auto SmButton = FSmButton::FromID(i);
        if(Manager->ButtonDownFrame(SmButton))
        {
            MessageHandler->OnControllerButtonPressed(GetKeyFrom(SmButton).GetFName(), PlatformUser, InputDevice, false);
        }
        if(Manager->ButtonUpFrame(SmButton))
        {
            MessageHandler->OnControllerButtonReleased(GetKeyFrom(SmButton).GetFName(), PlatformUser, InputDevice, false);
        }
    }
#else

    // Send axis data only while moving and an extra frame when axis values are supposedly 0
    if(Manager->MovementState->bMoving || Manager->MovementState->bOnMovementEndedFrame)
    {
        MessageHandler->OnControllerAnalog(
            SM_KEY_PREFIX_TEXT TEXT("Lateral"), 0,
            Manager->GetNormalizedTranslation().X
        );
        MessageHandler->OnControllerAnalog(
            SM_KEY_PREFIX_TEXT TEXT("Horizontal"), 0,
            Manager->GetNormalizedTranslation().Y
        );
        MessageHandler->OnControllerAnalog(
            SM_KEY_PREFIX_TEXT TEXT("Vertical"), 0,
            Manager->GetNormalizedTranslation().Z
        );
    
        MessageHandler->OnControllerAnalog(
            SM_KEY_PREFIX_TEXT TEXT("Pitch"), 0,
            Manager->GetNormalizedRotation().Pitch
        );
        MessageHandler->OnControllerAnalog(
            SM_KEY_PREFIX_TEXT TEXT("Yaw"), 0,
            Manager->GetNormalizedRotation().Yaw
        );
        MessageHandler->OnControllerAnalog(
            SM_KEY_PREFIX_TEXT TEXT("Roll"), 0,
            Manager->GetNormalizedRotation().Roll
        );
    }

    for(int i=0; i<Manager->GetButtons().Num(); ++i)
    {
        auto SmButton = FSmButton::FromID(i);
        if(Manager->ButtonDownFrame(SmButton))
        {
            MessageHandler->OnControllerButtonPressed(GetKeyFrom(SmButton).GetFName(), 0, false);
        }
        if(Manager->ButtonUpFrame(SmButton))
        {
            MessageHandler->OnControllerButtonReleased(GetKeyFrom(SmButton).GetFName(), 0, false);
        }
    }
#endif
}

void FSmInputDevice::SetMessageHandler(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler)
{
    MessageHandler = InMessageHandler;
}

void FSmInputDevice::SetChannelValue(int32 ControllerId, FForceFeedbackChannelType ChannelType, float Value)
{
}

void FSmInputDevice::SetChannelValues(int32 ControllerId, const FForceFeedbackValues& values)
{
}

bool FSmInputDevice::Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar)
{
    return false;
}

#undef AXIS_FLAG
#undef LOCTEXT_NAMESPACE
