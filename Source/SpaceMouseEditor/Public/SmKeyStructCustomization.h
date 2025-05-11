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
#include "IPropertyTypeCustomization.h"
#include "PropertyHandle.h"
#include "SmUeVersion.h"

/**
 * 
 */
class SPACEMOUSEEDITOR_API FSmKeyStructCustomization
    : public IPropertyTypeCustomization
{
public:
    FSmKeyStructCustomization();
    ~FSmKeyStructCustomization();
    
    virtual void CustomizeHeader(
        TSharedRef<class IPropertyHandle> StructPropertyHandle,
        class FDetailWidgetRow& HeaderRow,
        IPropertyTypeCustomizationUtils& StructCustomizationUtils
    ) override;
    
    virtual void CustomizeChildren(
        TSharedRef<IPropertyHandle> StructPropertyHandle,
        class IDetailChildrenBuilder& ChildBuilder,
        IPropertyTypeCustomizationUtils& StructCustomizationUtils
    ) override { };

    // New functions in UE 4.26
#if UE_VERSION >= MAKE_UE_VERSION(4, 26)
    
    virtual bool ShouldInlineKey() const override { return true; }
    
#endif
    
    void CustomizeHeaderOnlyWithButton(
        TSharedRef<class IPropertyHandle> StructPropertyHandle,
        class FDetailWidgetRow& HeaderRow,
        IPropertyTypeCustomizationUtils& StructCustomizationUtils,
        TSharedRef<SWidget> Button
    );
    
    static TSharedRef<IPropertyTypeCustomization> MakeInstance();

protected:

    TOptional<FKey> GetCurrentKey() const;
    void OnKeyChanged(TSharedPtr<FKey> SelectedKey);
    TSharedPtr<IPropertyHandle> PropertyHandle;
};
