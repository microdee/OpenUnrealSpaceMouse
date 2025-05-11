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



#include "SpaceMouseManager.h"
#include "SpaceMouseReader.h"

#include "hidapi.h"
#include "MovementState.h"
#include "DeviceTypes/DeviceFactory.h"
#include "DeviceTypes/SmDevice.h"

#if PLATFORM_MAC
#include "Mac/TDxWareReadingMethod.h"
#include "Mac/TDxWareButtonCapabilities.h"
#endif

#if WITH_EDITOR
#if !PLATFORM_MAC
#include "EngineGlobals.h"
#endif
#include "Engine/Engine.h"
#endif

void FSpaceMouseManager::Initialize()
{
    PrevAccumulatedData = AccumulatedData = {};
    MovementState = MakeShared<FSmMovementState>();
    Devices.Empty();

#if PLATFORM_MAC // On mac we have to use the 3DxWare SDK (at least pretend ;) )

    FSmDeviceInstantiation InstInfo {
        0, nullptr, [this]() { return GetUserSettings(); }
    };
    auto MacDevice = MakeShared<FSmDevice>(
        TEXT("3DxWare driver"),
        ESmModelConfidence::Unknown,
        MakeShared<FTDxWareButtonCapabilities>(),
        FTDxWareReadingMethod::GetSingleton(),
        InstInfo
    );
    Devices.Add(MacDevice);

#else // On windows we use HID (Linux is unknown yet)

    FSmDeviceFactory Factory {};
    Factory.OpenConnectedDevices([this]() { return GetUserSettings(); }, Devices);

#endif
    
    Enabled = true;
    DeviceOpened = Devices.Num() > 0;
}

bool FSpaceMouseManager::ButtonDownFrame(const EV3DCmd Button)
{
    auto Id = FSmButton::FromCmd(Button);
    return AccumulatedData.Buttons[Id] && !PrevAccumulatedData.Buttons[Id]; 
}

bool FSpaceMouseManager::ButtonUpFrame(const EV3DCmd Button)
{
    auto Id = FSmButton::FromCmd(Button);
    return !AccumulatedData.Buttons[Id] && PrevAccumulatedData.Buttons[Id]; 
}

void FSpaceMouseManager::Tick(float DeltaSecs)
{
    PrevAccumulatedData = AccumulatedData;
    AccumulatedData = NormalizedData = {};

    MovementState->AccumulationReset();
    
    for (auto SmDevice : Devices)
    {
        SmDevice->Tick(DeltaSecs);
        AccumulatedData += SmDevice->ProcessedData;
        NormalizedData += SmDevice->NormData;
        MovementState->Accumulate(SmDevice->MovementState);
    }

#if WITH_EDITOR
    if (GetUserSettings().bPrintDebug)
    {
        FString Message = TEXT("Connected SpaceMice: ") + FString::FromInt(Devices.Num());

        for (const auto& SmDevice : Devices)
        {
            Message += TEXT("\n    Device: ") + SmDevice->DeviceName;
        }
        
        GEngine->AddOnScreenDebugMessage(
            2000, 1.0, FColor::Cyan,
            Message
        );

        for(int i=0; i<AccumulatedData.Buttons.Num(); i++)
        {
            if(ButtonDownFrame(FSmButton::FromID(i)))
            {
                GEngine->AddOnScreenDebugMessage(
                    INDEX_NONE, 1.2, FColor::Emerald,
                    FSmButton::GetFriendlyNameOf(FSmButton::FromID(i))
                );
            }
        }
    }
#endif
}
