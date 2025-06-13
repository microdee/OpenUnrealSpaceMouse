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




#include "SpaceMouseReader/Public/CommonBehaviors.h"

FVector USmCommonBehaviors::GetOrbitingTranslationDelta(
    const FVector& pivot,
    const FRotator& currentRotation,
    FRotator rotationDelta,
    float distance,
    bool withRoll
) {
    if (!withRoll)
    {
        const float yawCorr = FMath::Abs(FMath::Cos(FMath::DegreesToRadians(currentRotation.Pitch)));
        rotationDelta.Yaw *= yawCorr;
    }

    const FMatrix orbitTr = FTransform(pivot).ToMatrixWithScale()
        * FTransform(rotationDelta).ToMatrixWithScale()
        * FTransform(FVector(-distance, 0, 0)).ToMatrixWithScale();

    FVector ret = orbitTr.TransformPosition(FVector::ZeroVector);
    ret.X = 0;
    return ret;
}
