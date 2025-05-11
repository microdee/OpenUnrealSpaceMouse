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
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CommonBehaviors.generated.h"

/**
 * 
 */
UCLASS()
class SPACEMOUSEREADER_API USmCommonBehaviors : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()
public:

    UFUNCTION(BlueprintPure, Category=SpaceMouse)
    static FVector GetOrbitingTranslationDelta(
        FVector Pivot,
        FRotator CurrentRotation,
        FRotator RotationDelta,
        float Distance,
        bool bWithRoll
    );
};
