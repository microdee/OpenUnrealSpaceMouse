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
    FVector Pivot,
    FRotator CurrentRotation,
    FRotator RotationDelta,
    float Distance,
    bool bWithRoll
) {
    if (!bWithRoll)
    {
        const float YawCorr = FMath::Abs(FMath::Cos(FMath::DegreesToRadians(CurrentRotation.Pitch)));
        RotationDelta.Yaw *= YawCorr;
    }

    const FMatrix OrbitTr = FTransform(Pivot).ToMatrixWithScale()
        * FTransform(RotationDelta).ToMatrixWithScale()
        * FTransform(FVector(-Distance, 0, 0)).ToMatrixWithScale();

    FVector Ret = OrbitTr.TransformPosition(FVector::ZeroVector);
    Ret.X = 0;
    return Ret;
}
