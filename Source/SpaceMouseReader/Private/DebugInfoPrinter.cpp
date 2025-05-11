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




#include "DebugInfoPrinter.h"

#include "hidapi.h"
#include "Engine/Engine.h"


FSmDebugInfoPrinter::FSmDebugInfoPrinter()
{
}

FSmDebugInfoPrinter::~FSmDebugInfoPrinter()
{
}

void FSmDebugInfoPrinter::Print(FString FriendlyDeviceName, const hid_device_info* DeviceInfo, int InternalID, bool bEnabled)
{
    if(!bEnabled || !DeviceInfo) return;
    
    auto message = FString::Printf(
        TEXT("Device: %s\n")
        TEXT("  Serial: %s\n")
        TEXT("  VID: 0x%04x\n")
        TEXT("  PID: 0x%04x\n")
        TEXT("  Report:\n")
        TEXT("    %s\n")
        TEXT("  Sorted:\n"),
        *FriendlyDeviceName,
        DeviceInfo->serial_number,
        DeviceInfo->vendor_id,
        DeviceInfo->product_id,
        *ReportAll
    );

    for(int i=0; i<Reports.Num(); i++)
    {
        auto ReportLine = Reports[i];
        message += TEXT("    ") + ReportLine + TEXT("\n");
    }
    GEngine->AddOnScreenDebugMessage(2010 + InternalID, 10.0, FColor::Orange, message);
    ReportAll.Empty();
}

void FSmDebugInfoPrinter::AppendReport(const uint8* Report, uint32 Size)
{
    ReportAll += FString::FromHexBlob(Report, Size);
}

void FSmDebugInfoPrinter::SetReport(int Index, const uint8* Report, uint32 Size)
{
    Reports[Index] = FString::FromHexBlob(Report, Size);
}
