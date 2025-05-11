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

#include "UObject/Object.h"
#include "TimerManager.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Misc/App.h"

#include "SpaceMouseData.generated.h"

/**
 * 
 */
UCLASS()
class SPACEMOUSERUNTIME_API USpaceMouseData : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
public:
    static bool bFrameRequested;
    static FTimerDelegate OnTickDel;

    UFUNCTION(
        BlueprintPure,
        Category = "SpaceMouse"
    )
    static void GetSpaceMouseDeltaAxes(
        FVector& DeltaTranslation,
        FRotator& DeltaRotation
    );
    
    UFUNCTION(
        BlueprintPure,
        Category = "SpaceMouse"
    )
    static void GetSpaceMouseAxes(
        FVector& NormalizedTranslation,
        FRotator& NormalizedRotation
    );

    UFUNCTION(
        BlueprintPure,
        Category = "SpaceMouse"
    )
    static bool GetSpaceMouseButtonState(int Id);

    UFUNCTION(
        BlueprintPure,
        Category = "SpaceMouse"
    )
    static bool GetSpaceMouseButtonDown(int Id);

    UFUNCTION(
        BlueprintPure,
        Category = "SpaceMouse"
    )
    static bool GetSpaceMouseButtonUp(int Id);
};
