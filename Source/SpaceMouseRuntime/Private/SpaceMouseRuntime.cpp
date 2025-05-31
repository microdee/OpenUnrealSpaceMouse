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

#include "SpaceMouseRuntime.h"
#include "Modules/ModuleManager.h"
#include "SpaceMouseRuntime/SmInputDevice.h"

void FSpaceMouseRuntimeModule::StartupModule()
{
	OnStartupModule.Broadcast();
	IModularFeatures::Get().RegisterModularFeature(GetModularFeatureName(), this);
}

void FSpaceMouseRuntimeModule::ShutdownModule()
{
	OnShutdownModule.Broadcast();
	IModularFeatures::Get().UnregisterModularFeature(GetModularFeatureName(), this);
}

TSharedPtr<IInputDevice> FSpaceMouseRuntimeModule::CreateInputDevice(
	TSharedRef<FGenericApplicationMessageHandler> const& messageHandler
) {
	return MakeShared<SpaceMouse::Runtime::FSmInputDevice>(messageHandler);
}
	
IMPLEMENT_MODULE(FSpaceMouseRuntimeModule, SpaceMouseRuntime)
