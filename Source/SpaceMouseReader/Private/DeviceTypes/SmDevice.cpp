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




#include "DeviceTypes/SmDevice.h"


#include "DebugInfoPrinter.h"
#include "MovementState.h"
#include "ProcessedDeviceOutput.h"
#include "DeviceTypes/ButtonCapabilities.h"
#include "ReadingMethod/ActiveHidSmDevice.h"
#include "ReadingMethod/DataReadingMethod.h"

FSmDevice::FSmDevice(
    const FString DeviceName,
    const ESmModelConfidence ModelConfidence,
    const TSharedPtr<FSmButtonCapabilities> Buttons,
    const TSharedPtr<FSmDataReadingMethod> DataReadingMethod,
    const FSmDeviceInstantiation& InstanceData
)   : DeviceName(DeviceName)
    , ModelConfidence(ModelConfidence)
    , InternalID(InstanceData.InternalID)
    , UserSettings(InstanceData.UserSettings)
    , Buttons(Buttons)
    , DataReadingMethod(DataReadingMethod)
    , HidDevice(InstanceData.HidDevice)
{
}

FSmDevice::~FSmDevice()
{
}

void FSmDevice::TickInit()
{
    if(!bInited)
    {
        bInited = true;
        ProcessedData = MakeShared<FSmProcessedDeviceOutput>();
        NormData = MakeShared<FSmProcessedDeviceOutput>();
        DebugInfoPrinter = MakeShared<FSmDebugInfoPrinter>();
        MovementState = MakeShared<FSmMovementState>();

        DataReadingMethod->OnDataReceived.AddLambda([this]()
        {
            DebugInfoPrinter->Print(
                DeviceName,
                HidDevice ? HidDevice->DeviceInfo : nullptr,
                InternalID,
                UserSettings().bPrintDebug
            );
        });
    }
}

void FSmDevice::Tick(float DeltaSeconds)
{
    TickInit();

    ProcessedData->Buttons = NormData->Buttons;
    FSmDataReadingOutput Output
    {
        ProcessedData,
        NormData,
        DebugInfoPrinter,
        HidDevice,
        MovementState,
        UserSettings()
    };
    DataReadingMethod->Tick(Output, DeltaSeconds);
    Buttons->TransformRawData(ProcessedData);
    // Buttons->TransformRawData(NormData); // NormData keeps the HID buttons from the device
}
