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
#include "Mcro/Delegates/EventDelegate.h"
#include "IInputDeviceModule.h"

class FSpaceMouseRuntimeModule : public IInputDeviceModule
{
public:
	
	Mcro::Delegates::TBelatedEventDelegate<void()> OnStartupModule;
	Mcro::Delegates::TBelatedEventDelegate<void()> OnShutdownModule;

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	virtual bool SupportsDynamicReloading() override { return true; }
	
	virtual TSharedPtr<IInputDevice> CreateInputDevice(
		TSharedRef<FGenericApplicationMessageHandler> const& messageHandler
	) override;
};
