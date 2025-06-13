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
#include "IInputDevice.h"
#include "SpaceMouseRuntime/RuntimeManager.h"
#include "InputCoreTypes.h"

class FGenericApplicationMessageHandler;

namespace SpaceMouse::Runtime
{
	class SPACEMOUSERUNTIME_API FSmInputDevice : public IInputDevice
	{
	public:
		static TMap<FKey, Buttons::ECmd> const& GetKeyButtonMap();
    
		FSmInputDevice(const TSharedRef<FGenericApplicationMessageHandler>& messageHandler);
		~FSmInputDevice();

		static FKey GetKeyFrom(Buttons::ECmd cmd);
		static Buttons::ECmd GetButtonFrom(FKey const& key);
		static FKeyDetails GetKeyDetailsFrom(Buttons::ECmd cmd);
		static void RegisterSmButtons();
    
		virtual void Tick(float) override {}
		virtual void SendControllerEvents() override;
		virtual void SetMessageHandler(const TSharedRef<FGenericApplicationMessageHandler>& messageHandler) override;
		virtual void SetChannelValue(int32, FForceFeedbackChannelType, float) override {}
		virtual void SetChannelValues(int32, FForceFeedbackValues const&) override {}
		virtual bool Exec(UWorld*, const TCHAR*, FOutputDevice&) override { return false; }
    
	private:
		/* Message handler */
		TSharedRef<FGenericApplicationMessageHandler> MessageHandler;
	};
}