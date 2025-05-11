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




#include "ReadingMethod/ActiveHidSmDevice.h"

#include "hidapi.h"


FActiveHidSmDevice::FActiveHidSmDevice(hid_device_info* DeviceInfo)
    : DeviceInfo(DeviceInfo)
{
    Device = hid_open_path(DeviceInfo->path);
    bDeviceOpened = Device != nullptr;
    if (bDeviceOpened) hid_set_nonblocking(Device, 1);
}

FActiveHidSmDevice::~FActiveHidSmDevice()
{
    if(Device) hid_close(Device);
}

int FActiveHidSmDevice::Read(uint8* data, size_t length) const
{
    return hid_read(Device, data, length);
}
