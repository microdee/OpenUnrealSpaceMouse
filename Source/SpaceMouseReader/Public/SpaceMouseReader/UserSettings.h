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
#include "Curves/RichCurve.h"
#include "UserSettings.generated.h"

USTRUCT(BlueprintType)
struct FSmMovementSettings
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadWrite, Category=SpaceMouse)
    FVector XAxisMap {0, -1,  0}; // Pitch
    
    UPROPERTY(BlueprintReadWrite, Category=SpaceMouse)
    FVector YAxisMap {1,  0,  0}; // Yaw
    
    UPROPERTY(BlueprintReadWrite, Category=SpaceMouse)
    FVector ZAxisMap {0,  0, -1}; // Roll
    
    UPROPERTY(BlueprintReadWrite, Category=SpaceMouse)
    float UnitsPerSec = 1000;
    
    UPROPERTY(BlueprintReadWrite, Category=SpaceMouse)
    FRichCurve Curve {};
};

USTRUCT(BlueprintType)
struct SPACEMOUSEREADER_API FSmUserSettings
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category=SpaceMouse)
    bool bPrintDebug = false;
    
    UPROPERTY(BlueprintReadWrite, Category=SpaceMouse)
    float MovementTimeTolerance = 0.25;

    UPROPERTY(BlueprintReadWrite, Category=SpaceMouse)
    FSmMovementSettings Translation
    {
        {0, -1,  0},
        {1,  0,  0},
        {0,  0, -1},
        1000.0f
    };

    UPROPERTY(BlueprintReadWrite, Category=SpaceMouse)
    FSmMovementSettings Rotation
    {
        {1,  0,  0},
        {0,  0,  1},
        {0, -1,  0},
        270.0f
    };
};
