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
 * This HID data format has the translation and rotation in a single report
 */
class SPACEMOUSEREADER_API FSingleReportTransRotMethod : public FHidDataReadingMethod
{
public:
    FSingleReportTransRotMethod();
    ~FSingleReportTransRotMethod();
    
    virtual void ReadData(FSmDataReadingOutput& Output, float DeltaSecs, uint8* Report) override;
    virtual int GetReportSize() override { return 13; }
};
