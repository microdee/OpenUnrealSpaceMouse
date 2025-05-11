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

#include "SmUserSettings.h"

enum class ESmModelConfidence : uint8;
class FSmDevice;
struct FSmDeviceInstantiation;

typedef TFunction<TSharedPtr<FSmDevice>(const FSmDeviceInstantiation&)> FSmDeviceCreator;


#define JOIN_VIDPID(vid, pid) ((vid) | ((pid) << 16))
#define GET_VID(vidpid) ((vidpid) & 0x0000FFFF)
#define GET_PID(vidpid) (((vidpid) & 0xFFFF0000) >> 16)

/**
 * 
 */
class SPACEMOUSEREADER_API FSmDeviceFactory
{
public:
    FSmDeviceFactory();
    ~FSmDeviceFactory();

    TMap<uint32 /* vid, pid */, FSmDeviceCreator> KnownModels;
    void AddModel(uint16 Vid, uint16 Pid, FSmDeviceCreator Creator);

    void OpenConnectedDevices(const TFunction<FSmUserSettings()>& Settings, TArray<TSharedPtr<FSmDevice>>& Output);
};
