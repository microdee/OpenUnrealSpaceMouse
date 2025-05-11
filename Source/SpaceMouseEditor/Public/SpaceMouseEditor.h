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
#include "Modules/ModuleManager.h"
#include "PropertyEditorDelegates.h"
#include "SmEditorManager.h"
#include "LevelEditorViewport.h"
#include "SpaceMouseConfig.h"

//General Log
DECLARE_LOG_CATEGORY_EXTERN(LogSpaceMouseEditor, Log, All);

class FSpaceMouseEditorModule : public IModuleInterface
{
private:

    bool HandleSettingsSaved();
    void RegisterSettings();
    void UnregisterSettings();
    
	TSet<FName> RegisteredPropertyTypes;
	void RegisterPropertyTypeCustomizations();
	void RegisterCustomPropertyTypeLayout(FName PropertyTypeName, FOnGetPropertyTypeCustomizationInstance PropertyTypeLayoutDelegate );

public:

    /** IModuleInterface implementation */
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
    virtual bool SupportsDynamicReloading() override { return true; }
    
    FSmEditorManager SmManager;
    class FSpaceMouseReaderModule* ReaderModule;
};
