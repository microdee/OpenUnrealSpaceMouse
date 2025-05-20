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

#include "SpaceMouseReader/Hidapi.h"
#include "SpaceMouseReader/DeviceId.h"
#include "Mcro/Common.h"

namespace SpaceMouse::Reader::Hid
{
	using namespace Mcro::Common;

	class SPACEMOUSEREADER_API FHidDeviceId : public TInherit<IDeviceIdBase>
	{
	public:
		union
		{
			struct
			{
				uint16 Vid;
				uint16 Pid;
			} VidPid;
			
			uint32 Hash;
		};
		FHidDeviceId();
		FHidDeviceId(uint16 vid, uint16 pid);
		virtual FString ToString() const override;
		virtual uint32 GetHash() const override;
	};

	class SPACEMOUSEREADER_API FHidDevicePath : public TInherit<ISpecificDeviceId>
	{
	public:
		FHidDevicePath(const TCHAR* name);
		FHidDevicePath(std::string const& name);
		FString Name;
		virtual FString ToString() const override;
		virtual uint32 GetHash() const override;
	};
	
	struct SPACEMOUSEREADER_API FHidDeviceInfo
	{
		std::string  Path {};
		uint16       VendorId = 0;
		uint16       ProductId = 0;
		std::wstring SerialNumber {};
		uint16       ReleaseNumber = 0;
		std::wstring ManufacturerString {};
		std::wstring ProductString {};
		uint16       UsagePage = 0;
		uint16       Usage = 0;
		int          InterfaceNumber = 0;
		hid_bus_type BusType = HID_API_BUS_UNKNOWN;

		FString ToString() const;
		friend auto operator << (YAML::Emitter& emitter, FHidDeviceInfo const& v) -> YAML::Emitter&;

		FHidDeviceInfo() {}
		FHidDeviceInfo(hid_device_info const& source);
		
		FHidDeviceInfo(const FHidDeviceInfo&) = default;
		FHidDeviceInfo(FHidDeviceInfo&&) noexcept = default;
		FHidDeviceInfo& operator = (const FHidDeviceInfo&) = default;
		FHidDeviceInfo& operator = (FHidDeviceInfo&&) noexcept = default;
	};

	class SPACEMOUSEREADER_API FHidError : public IError
	{
	public:
		FHidError(hid_device* dev = nullptr);
		FHidError(const wchar_t* message);
		FString HidErrorMessage;
		TOptional<FHidDeviceInfo> DeviceInfo;

		virtual void SerializeMembers(YAML::Emitter& emitter) const override;
		virtual auto CreateErrorWidget() -> TSharedRef<SErrorDisplay> override;

		template <typename Self>
		SelfRef<Self> WithDeviceInfo(this Self&& self, FHidDeviceInfo const& deviceInfo)
		{
			DeviceInfo.Emplace(deviceInfo);
			return self.SharedThis(&self);
		}
	};

	FCanFail SPACEMOUSEREADER_API EnumerateDevices(TFunction<void(int32, FHidDeviceInfo const&)> const& function);
	
	class SPACEMOUSEREADER_API FScopedHidDevice
	{
		hid_device* Device = nullptr;
		FHidDeviceInfo DeviceInfo {};
		FCanFail DeviceOpenResult;
		
	public:

		FScopedHidDevice(FScopedHidDevice&& from);
		FScopedHidDevice(const FScopedHidDevice&) = delete;
		FScopedHidDevice& operator = (const FScopedHidDevice&) = delete;
		
		FScopedHidDevice(FHidDeviceInfo const& deviceInfo);
		~FScopedHidDevice();
		
		TMaybe<int32> Read(TArray<uint8>& output) const;

		FORCEINLINE bool IsOpened() const { return DeviceOpenResult; }
	};
}
