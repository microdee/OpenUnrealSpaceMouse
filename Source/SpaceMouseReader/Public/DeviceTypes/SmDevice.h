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
#include "SmUserSettings.h"

class FSmMovementState;
class FSmDebugInfoPrinter;
class FSmDataReadingMethod;
class FSmButtonCapabilities;
class FActiveHidSmDevice;
struct FSmProcessedDeviceOutput;

/**
 * Used to pass dynamic data to 
 */
struct SPACEMOUSEREADER_API FSmDeviceInstantiation
{
    int InternalID = 0;
    TSharedPtr<FActiveHidSmDevice> HidDevice;
    TFunction<FSmUserSettings()> UserSettings;
};

enum class ESmModelConfidence : uint8
{
    Tested,
    TestedViaFeedback,
    UntestedShouldWork,
    Unknown,
    Unsupported
};

/**
 * Root object for containing spacemouse components 
 */
class SPACEMOUSEREADER_API FSmDevice
{
public:
    FSmDevice(
        const FString DeviceName,
        const ESmModelConfidence ModelConfidence,
        const TSharedPtr<FSmButtonCapabilities> Buttons,
        const TSharedPtr<FSmDataReadingMethod> DataReadingMethod,
        const FSmDeviceInstantiation& InstanceData
    );
    ~FSmDevice();

    FString DeviceName;
    ESmModelConfidence ModelConfidence;
    int InternalID;
    TFunction<FSmUserSettings()> UserSettings;
    TSharedPtr<FSmButtonCapabilities> Buttons;
    TSharedPtr<FSmDataReadingMethod> DataReadingMethod;
    TSharedPtr<FActiveHidSmDevice> HidDevice;
    TSharedPtr<FSmProcessedDeviceOutput> NormData;
    TSharedPtr<FSmProcessedDeviceOutput> ProcessedData;
    TSharedPtr<FSmMovementState> MovementState;
    TSharedPtr<FSmDebugInfoPrinter> DebugInfoPrinter;

    void Tick(float DeltaSeconds);

private:

    bool bInited = false;
    void TickInit();
};
