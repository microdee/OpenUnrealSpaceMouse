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

#include "Containers/StaticArray.h"

struct hid_device_info;

/**
 * 
 */
class SPACEMOUSEREADER_API FSmDebugInfoPrinter
{
public:

    static inline const int ReportCount = 4;
    
    FSmDebugInfoPrinter();
    ~FSmDebugInfoPrinter();

    TStaticArray<FString, ReportCount> Reports;
    FString ReportAll {};
    
    void Print(FString FriendlyDeviceName, const hid_device_info* DeviceInfo, int InternalID, bool bEnabled);
    void AppendReport(const uint8* Report, uint32 Size);
    void SetReport(int Index, const uint8* Report, uint32 Size);
};
