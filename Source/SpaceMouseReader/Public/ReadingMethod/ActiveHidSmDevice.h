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

struct hid_device_;
typedef struct hid_device_ hid_device;
struct hid_device_info;

/**
 * 
 */
class SPACEMOUSEREADER_API FActiveHidSmDevice
{
public:
    FActiveHidSmDevice(hid_device_info* DeviceInfo);
    ~FActiveHidSmDevice();
    
    int Read(uint8* data, size_t length) const;
    
    hid_device* Device = nullptr;
    hid_device_info* DeviceInfo = nullptr;
    bool bDeviceOpened = false;
};
