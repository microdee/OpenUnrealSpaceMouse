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
//#include "App.h"
//#include "Object.h"
#include "SmInputDevice.h"
#include "Modules/ModuleManager.h"
#include "TimerManager.h"
#include "SpaceMouseReader.h"
#include "SpaceMouseData.h"


#define LOCTEXT_NAMESPACE "FSpaceMouseRuntimeModule"

//General Log
DEFINE_LOG_CATEGORY(SpaceMouseRuntime);

void FSpaceMouseRuntimeModule::StartupModule()
{
    IModularFeatures::Get().RegisterModularFeature(GetModularFeatureName(), this);
}

void FSpaceMouseRuntimeModule::ShutdownModule()
{
    IModularFeatures::Get().UnregisterModularFeature(GetModularFeatureName(), this);
}

TSharedPtr<IInputDevice> FSpaceMouseRuntimeModule::CreateInputDevice(
    const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler)
{
    return MakeShared<FSmInputDevice>(InMessageHandler);
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FSpaceMouseRuntimeModule, SpaceMouseRuntime)
