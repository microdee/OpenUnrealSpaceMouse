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




#include "SmKeyStructCustomization.h"
#include "DetailWidgetRow.h"
//#include "InputSettingsDetails.h"
#include "SmKey.h"
#include "SSmKeySelector.h"
#include "UObject/UnrealType.h"

#define LOCTEXT_NAMESPACE "FSmKeyStructCustomization"

// from Engine/Source/Editor/DetailCustomizations/Private/InputSettingsDetails.h
// which is a private header and it would be nasty to include it here 
namespace InputConstants
{
    const FMargin PropertyPadding(2.0f, 0.0f, 2.0f, 0.0f);
    const float TextBoxWidth = 250.0f;
    const float ScaleBoxWidth = 50.0f;
}

/* FKeyStructCustomization static interface
 *****************************************************************************/

TSharedRef<IPropertyTypeCustomization> FSmKeyStructCustomization::MakeInstance( )
{
    return MakeShareable(new FSmKeyStructCustomization);
}

/* IPropertyTypeCustomization interface
 *****************************************************************************/

FSmKeyStructCustomization::FSmKeyStructCustomization()
{
}

FSmKeyStructCustomization::~FSmKeyStructCustomization()
{
}

void FSmKeyStructCustomization::CustomizeHeader( TSharedRef<class IPropertyHandle> StructPropertyHandle, class FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils )
{
    PropertyHandle = StructPropertyHandle;

    // create struct header
    HeaderRow.NameContent()
    [
        StructPropertyHandle->CreatePropertyNameWidget()
    ]
    . ValueContent()
    . MinDesiredWidth(125.0f)
    . MaxDesiredWidth(325.0f)
    [
        SNew(SSmKeySelector)
        . CurrentKey(this, &FSmKeyStructCustomization::GetCurrentKey)
        . OnKeyChanged(this, &FSmKeyStructCustomization::OnKeyChanged)
        . Font(StructCustomizationUtils.GetRegularFont())
        . AllowClear(!StructPropertyHandle->GetProperty()->HasAnyPropertyFlags(CPF_NoClear))
        . FilterBlueprintBindable(false)
    ];
}

void FSmKeyStructCustomization::CustomizeHeaderOnlyWithButton(TSharedRef<class IPropertyHandle> StructPropertyHandle, class FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils, TSharedRef<SWidget> Button)
{
    PropertyHandle = StructPropertyHandle;

    // create struct header
    HeaderRow.NameContent()
    . MinDesiredWidth(125.0f)
    . MaxDesiredWidth(325.0f)
    [
        SNew(SHorizontalBox)
        + SHorizontalBox::Slot()
        . Padding(InputConstants::PropertyPadding)
        //.AutoWidth()
        [
            SNew(SSmKeySelector)
            . CurrentKey(this, &FSmKeyStructCustomization::GetCurrentKey)
            . OnKeyChanged(this, &FSmKeyStructCustomization::OnKeyChanged)
            . Font(StructCustomizationUtils.GetRegularFont())
            . AllowClear(!StructPropertyHandle->GetProperty()->HasAnyPropertyFlags(CPF_NoClear))
        ]
        + SHorizontalBox::Slot()
        . Padding(InputConstants::PropertyPadding)
        . HAlign(HAlign_Center)
        . VAlign(VAlign_Center)
        . AutoWidth()
        [
            Button
        ]
    ];
}

TOptional<FKey> FSmKeyStructCustomization::GetCurrentKey() const
{
    TArray<void*> StructPtrs;
    PropertyHandle->AccessRawData(StructPtrs);

    if (StructPtrs.Num() > 0)
    {
        FSmKey* SelectedKey = static_cast<FSmKey*>(StructPtrs[0]);

        if (SelectedKey)
        {
            for(int32 StructPtrIndex = 1; StructPtrIndex < StructPtrs.Num(); ++StructPtrIndex)
            {
                if (*(FSmKey*)StructPtrs[StructPtrIndex] != *SelectedKey)
                {
                    return TOptional<FKey>();
                }
            }

            return (*SelectedKey).Key;
        }
    }

    return FKey();
}

void FSmKeyStructCustomization::OnKeyChanged(TSharedPtr<FKey> SelectedKey)
{
    PropertyHandle->SetValueFromFormattedString(SelectedKey->ToString());
}

#undef LOCTEXT_NAMESPACE
