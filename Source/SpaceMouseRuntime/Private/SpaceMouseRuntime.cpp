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

#include "CoreMinimal.h"
#include "IInputDeviceModule.h"
#include "Modules/ModuleManager.h"
#include "SpaceMouseRuntime/SmInputDevice.h"

class FSpaceMouseRuntimeModule : public IInputDeviceModule
{
public:

    /** IModuleInterface implementation */
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
    virtual bool SupportsDynamicReloading() override { return true; }
    
    virtual TSharedPtr<IInputDevice> CreateInputDevice(
        TSharedRef<FGenericApplicationMessageHandler> const& messageHandler
    ) override;
};

void FSpaceMouseRuntimeModule::StartupModule()
{
    IModularFeatures::Get().RegisterModularFeature(GetModularFeatureName(), this);
}

void FSpaceMouseRuntimeModule::ShutdownModule()
{
    IModularFeatures::Get().UnregisterModularFeature(GetModularFeatureName(), this);
}

TSharedPtr<IInputDevice> FSpaceMouseRuntimeModule::CreateInputDevice(
    TSharedRef<FGenericApplicationMessageHandler> const& messageHandler
) {
    return MakeShared<SpaceMouse::Runtime::FSmInputDevice>(messageHandler);
}
    
IMPLEMENT_MODULE(FSpaceMouseRuntimeModule, SpaceMouseRuntime)
