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

#include "DataReadingMethod.h"
#include "SmUserSettings.h"

/**
 * Class encapsulating a method to convert raw HID input into usable data
 */
class SPACEMOUSEREADER_API FHidDataReadingMethod : public FSmDataReadingMethod
{
public:
    FHidDataReadingMethod();
    virtual ~FHidDataReadingMethod();
    
    virtual int GetReportSize() { return 7; }
    virtual int GetReportCount() { return 4; }

    virtual void Tick(FSmDataReadingOutput& Output, float DeltaSecs) override;
    virtual void ReadData(FSmDataReadingOutput& Output, float DeltaSecs, uint8* Report) = 0;

protected:
    
    uint8 OutputBuffer[80];
    
    void ApplyButtons(FSmDataReadingOutput& Output, uint8* Report, int ReportID = 3);
};
