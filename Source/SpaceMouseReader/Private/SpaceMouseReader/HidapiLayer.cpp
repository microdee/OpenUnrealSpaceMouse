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

#include "SpaceMouseReader/HidapiLayer.h"
#include "SpaceMouseReader.h"

DECLARE_LOG_CATEGORY_CLASS(LogSpaceMouseHid, Log, Log);

namespace SpaceMouse::Reader::Hid
{
	TObserveModule<FSpaceMouseReaderModule> GObserveModule {
		{
			.OnStartup = []
			{
				int32 result = hid_init();
				ERROR_CLOG(result < 0, LogSpaceMouseHid, Error, IError::Make(new FHidError())
					->WithMessage(TEXT_"Couldn't initialize hidapi library.")
					->WithDetails(TEXT_"Space-mice navigation devices will not work over USB")
					->AsFatal()
				)
			},
			.OnShutdown = []
			{
				int32 result = hid_exit();
				ERROR_CLOG(result < 0, LogSpaceMouseHid, Error, IError::Make(new FHidError())
					->WithMessage(TEXT_"hidapi library didn't shut down correctly.")
					->AsFatal()
				)
			}
		}
	};
	
	FHidDeviceId::FHidDeviceId() : Hash(0) {}
	FHidDeviceId::FHidDeviceId(uint16 vid, uint16 pid) : VidPid(vid, pid) {}

	FString FHidDeviceId::ToString() const
	{
		UE_LOG(LogSpaceMouseHid, Display, TEXT_"");
		return TEXT_"vid: {0}, pid: {1}" _FMT(VidPid.Vid, VidPid.Pid);
	}

	uint32 FHidDeviceId::GetHash() const
	{
		return Hash;
	}

	FHidDevicePath::FHidDevicePath(const TCHAR* name)
		: Name(name)
	{}

	FHidDevicePath::FHidDevicePath(std::string const& name)
		: Name(UnrealConvert(name))
	{}

	FString FHidDevicePath::ToString() const
	{
		return Name;
	}

	uint32 FHidDevicePath::GetHash() const
	{
		return GetTypeHash(Name);
	}

	FString FHidDeviceInfo::ToString() const
	{
		YAML::Emitter emitter;
		emitter << *this;
		return UTF8_TO_TCHAR(emitter.c_str());
	}

	auto operator << (YAML::Emitter& emitter, FHidDeviceInfo const& v) -> YAML::Emitter&
	{
		FMap map(emitter);
		GENERATE_YAML_MAP(emitter, v.,
			Path,
			VendorId,
			ProductId,
			SerialNumber,
			ReleaseNumber,
			ManufacturerString,
			ProductString,
			UsagePage,
			Usage,
			InterfaceNumber,
			BusType
		);
		return emitter;
	}

	FHidDeviceInfo::FHidDeviceInfo(hid_device_info const& source)
		: Path(               source.path                )
		, VendorId(           source.vendor_id           )
		, ProductId(          source.product_id          )
		, SerialNumber(       source.serial_number       )
		, ReleaseNumber(      source.release_number      )
		, ManufacturerString( source.manufacturer_string )
		, ProductString(      source.product_string      )
		, UsagePage(          source.usage_page          )
		, Usage(              source.usage               )
		, InterfaceNumber(    source.interface_number    )
		, BusType(            source.bus_type            )
	{}

	FHidError::FHidError(hid_device* dev)
	{
		HidErrorMessage = WCHAR_TO_TCHAR(hid_error(dev));
		IError::AddAppendix(TEXT_"HidErrorMessage", HidErrorMessage);
	}

	FHidError::FHidError(const wchar_t* message)
	{
		HidErrorMessage = WCHAR_TO_TCHAR(message);
		IError::AddAppendix(TEXT_"HidErrorMessage", HidErrorMessage);
	}

	void FHidError::SerializeMembers(YAML::Emitter& emitter) const
	{
		if (DeviceInfo.IsSet())
			emitter << YAML::Key << "Device" << YAML::Value << DeviceInfo.GetValue();
		IError::SerializeMembers(emitter);
	}

	TSharedRef<SErrorDisplay> FHidError::CreateErrorWidget()
	{
		FString deviceInfo = DeviceInfo.IsSet()
			? DeviceInfo.GetValue()
			: FString();
		
		return SNew(SErrorDisplay)
			.Error(SharedThis(this))
			.PostDetails()
			[
				SErrorDisplay::ExpandableTextWidget(INVTEXT_"Device", deviceInfo)
			]
		;
	}

	FCanFail EnumerateDevices(TFunction<FCanFail(int32, FHidDeviceInfo const&)> const& function)
	{
		auto devInfo = hid_enumerate(0, 0);
		auto firstDevInfo = devInfo;
		ASSERT_RETURN(devInfo, FHidError())
			->WithMessage(TEXT_"Couldn't enumerate HID devices.")
			->AsFatal()
			->BreakDebugger()
			->WithCppStackTrace()
			->ERROR_LOG(LogSpaceMouseHid, Error)
		;

		ON_SCOPE_EXIT { hid_free_enumeration(firstDevInfo); };

		int order = 0;
		while (devInfo)
		{
			PROPAGATE_FAIL(function(order, *devInfo));
			devInfo = devInfo->next;
			order++;
		}

		return Success();
	}

	FScopedHidDevice::FScopedHidDevice(FScopedHidDevice&& from)
		: Device(from.Device)
		, DeviceInfo(MoveTemp(from.DeviceInfo))
		, DeviceOpenResult(from.DeviceOpenResult)
	{}

	FScopedHidDevice::FScopedHidDevice(FHidDeviceInfo const& deviceInfo)
		: DeviceInfo(deviceInfo)
	{
		DeviceOpenResult = Success();
		Device = hid_open_path(deviceInfo.Path.c_str());
		if (UNLIKELY(!Device))
		{
			DeviceOpenResult = IError::Make(new FHidError())
				->AsFatal()->WithLocation()
				->WithMessage(TEXT_"Couldn't open HID device.")
				->WithDeviceInfo(deviceInfo)
				->ERROR_LOG(LogSpaceMouseHid, Error);
			return;
		}
		if (hid_set_nonblocking(Device, 1) < 0)
		{
			DeviceOpenResult = IError::Make(new FHidError())
				->AsFatal()->WithLocation()
				->WithMessage(TEXT_"Couldn't set HID device to be non-blocking.")
				->WithDeviceInfo(deviceInfo)
				->ERROR_LOG(LogSpaceMouseHid, Error);
			hid_close(Device);
			Device = nullptr;
		}
	}

	FScopedHidDevice::~FScopedHidDevice()
	{
		if (Device) hid_close(Device);
	}

	TMaybe<int32> FScopedHidDevice::Read(TArray<uint8>& output) const
	{
		ASSERT_RETURN(Device);
		
		int32 result = hid_read(Device, output.GetData(), output.Num());
		ASSERT_RETURN(result >= 0, FHidError(Device))
			->WithMessage(TEXT_"Error while reading from HID device")
			->WithDeviceInfo(DeviceInfo);
		return result;
	}
}
