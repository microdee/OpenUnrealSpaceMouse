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
#include "HidDataReadingMethod.h"

/**
 * This HID data format separates translation and rotation into separate reports
 */
class SPACEMOUSEREADER_API FSeparateReportTransRotMethod : public FHidDataReadingMethod
{
public:
    FSeparateReportTransRotMethod();
    ~FSeparateReportTransRotMethod();

    virtual void ReadData(FSmDataReadingOutput& Output, float DeltaSecs, uint8* Report) override;
};
