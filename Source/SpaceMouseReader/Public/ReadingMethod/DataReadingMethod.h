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
struct FRichCurve;
struct FSmProcessedDeviceOutput;
class FSmDebugInfoPrinter;
class FActiveHidSmDevice;

DECLARE_MULTICAST_DELEGATE(FDataReadEvent)

struct SPACEMOUSEREADER_API FSmDataReadingOutput
{
    TSharedPtr<FSmProcessedDeviceOutput> ProcessedData;
    TSharedPtr<FSmProcessedDeviceOutput> NormData;
    TSharedPtr<FSmDebugInfoPrinter> Debug;
    TSharedPtr<FActiveHidSmDevice> HidDevice;
    TSharedPtr<FSmMovementState> MovementState;
    FSmUserSettings Settings;
};

/**
 * Base class to encapsulate and configure input data to be used by the device
 */
class SPACEMOUSEREADER_API FSmDataReadingMethod
{
public:
    FSmDataReadingMethod();
    virtual ~FSmDataReadingMethod();
    
    FDataReadEvent OnDataReceived;
    
    virtual int GetAxisResolution() { return 350; }
    static float GetCurvedFloat(const FRichCurve* curve, float ff);
    
    virtual void Tick(FSmDataReadingOutput& Output, float DeltaSecs);

protected:

    static void TickMovementState(FSmDataReadingOutput& Output, float DeltaSecs);

    static void ApplyTranslation(FSmDataReadingOutput& Output, float fx, float fy, float fz, float DeltaSecs);
    static void ApplyRotation(FSmDataReadingOutput& Output, float fp, float fy, float fr, float DeltaSecs);
};
