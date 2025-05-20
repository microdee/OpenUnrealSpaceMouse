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




#include "SpaceMouseRuntime/SmInputDevice.h"

namespace SpaceMouse::Runtime
{
	TMap<FKey, Buttons::ECmd> KeyButtonMap;
	
	FKey GTranslationX;
	FKey GTranslationY;
	FKey GTranslationZ;
	
	FKey GRotationPitch;
	FKey GRotationYaw;
	FKey GRotationRoll;

	TMap<FKey, Buttons::ECmd> const& FSmInputDevice::GetKeyButtonMap()
	{
		return KeyButtonMap;
	}

	FSmInputDevice::FSmInputDevice(const TSharedRef<FGenericApplicationMessageHandler>& messageHandler)
		: MessageHandler(messageHandler)
	{
		RegisterSmButtons();
	}

	FSmInputDevice::~FSmInputDevice()
	{
	}

#define SM_KEY_PREFIX "OpenUnrealSpaceMouse_"
#define SM_KEY_PREFIX_TEXT_ TEXT_ "OpenUnrealSpaceMouse_"

	FKey FSmInputDevice::GetKeyFrom(Buttons::ECmd cmd)
	{
		static FString GlobalPrefix = SM_KEY_PREFIX_TEXT_;
		return FKey(*(GlobalPrefix + EnumToStringCopy(cmd)));
	}

	Buttons::ECmd FSmInputDevice::GetButtonFrom(FKey const& key)
	{
		if(KeyButtonMap.Contains(key)) return KeyButtonMap[key];
		return Buttons::ECmd::Noop;
	}

	FKeyDetails FSmInputDevice::GetKeyDetailsFrom(Buttons::ECmd cmd)
	{
		FString longName = TEXT_"SpaceMouse {0}" _FMT(Buttons::GetFriendlyName(cmd));
		return FKeyDetails(
			GetKeyFrom(cmd),
			FText::FromString(longName),
			FKeyDetails::GamepadKey,
			TEXT_"SpaceMouse",
			FText::FromStringView(Buttons::GetFriendlyName(cmd))
		);
	}

	FKeyDetails AxisDetail(FString const& axis, uint32 flags = FKeyDetails::Axis1D)
	{
		return {
			FKey(*(SM_KEY_PREFIX_TEXT_ + axis)),
			FText::FromString(axis),
			FKeyDetails::GamepadKey | flags,
			TEXT_"SpaceMouse"
		};
	}

	void FSmInputDevice::RegisterSmButtons()
	{
		// register menu category
		EKeys::AddMenuCategoryDisplayInfo(
			TEXT_"SpaceMouse",
			INVTEXT_"SpaceMouse",
			//TEXT("GraphEditor.KeyEvent_16x") // TODO: make our own palette icon? I don't think it's possible through this API
			TEXT_"StaticMeshEditor.SetShowSockets"
		);

		// register axes
		auto trX = AxisDetail(STRING_"Lateral");
		auto trY = AxisDetail(STRING_"Horizontal");
		auto trZ = AxisDetail(STRING_"Vertical");
		GTranslationX = trX.GetKey();
		GTranslationY = trY.GetKey();
		GTranslationZ = trZ.GetKey();
		EKeys::AddKey(trX);
		EKeys::AddKey(trY);
		EKeys::AddKey(trZ);
	
		auto rotP = AxisDetail(STRING_"Pitch");
		auto rotY = AxisDetail(STRING_"Yaw");
		auto rotR = AxisDetail(STRING_"Roll");
		GRotationPitch = rotP.GetKey();
		GRotationYaw = rotY.GetKey();
		GRotationRoll = rotR.GetKey();
		EKeys::AddKey(rotP);
		EKeys::AddKey(rotY);
		EKeys::AddKey(rotR);

		// register buttons
		for (int i = 0; i < static_cast<int>(Buttons::ECmd::COUNT); ++i)
		{
			auto key = GetKeyDetailsFrom(Buttons::AsCmd(i));
			EKeys::AddKey(key);
			KeyButtonMap.Add(key.GetKey(), Buttons::AsCmd(i));
		}
	}

	void FSmInputDevice::SendControllerEvents()
	{
		auto& manager = FRuntimeManager::Get();
		auto& deviceMapper = IPlatformInputDeviceMapper::Get();
		auto platformUser = PLATFORMUSERID_NONE;
		auto inputDevice = INPUTDEVICEID_NONE;
		
		manager.TickManager(FApp::GetDeltaTime());
	
		deviceMapper.RemapControllerIdToPlatformUserAndDevice(0, OUT platformUser, OUT inputDevice);

		// Send axis data only while moving and an extra frame when axis values are supposedly 0
		if(manager.MovementState.Moving || manager.MovementState.bOnMovementEndedFrame)
		{
			MessageHandler->OnControllerAnalog(
				SM_KEY_PREFIX_TEXT_"Lateral",
				platformUser, inputDevice,
				manager.GetNormalizedTranslation().X
			);
			MessageHandler->OnControllerAnalog(
				SM_KEY_PREFIX_TEXT_"Horizontal", 
				platformUser, inputDevice,
				manager.GetNormalizedTranslation().Y
			);
			MessageHandler->OnControllerAnalog(
				SM_KEY_PREFIX_TEXT_"Vertical", 
				platformUser, inputDevice,
				manager.GetNormalizedTranslation().Z
			);
	
			MessageHandler->OnControllerAnalog(
				SM_KEY_PREFIX_TEXT_"Pitch", 
				platformUser, inputDevice,
				manager.GetNormalizedRotation().Pitch
			);
			MessageHandler->OnControllerAnalog(
				SM_KEY_PREFIX_TEXT_"Yaw", 
				platformUser, inputDevice,
				manager.GetNormalizedRotation().Yaw
			);
			MessageHandler->OnControllerAnalog(
				SM_KEY_PREFIX_TEXT_"Roll", 
				platformUser, inputDevice,
				manager.GetNormalizedRotation().Roll
			);
		}

		for (auto const& pair : manager.Buttons)
		{
			if (pair.Value.OnDown())
				MessageHandler->OnControllerButtonPressed(GetKeyFrom(pair.Key).GetFName(), platformUser, inputDevice, false);
			if (pair.Value.OnUp())
				MessageHandler->OnControllerButtonReleased(GetKeyFrom(pair.Key).GetFName(), platformUser, inputDevice, false);
		}
	}

	void FSmInputDevice::SetMessageHandler(const TSharedRef<FGenericApplicationMessageHandler>& messageHandler)
	{
		MessageHandler = messageHandler;
	}
}