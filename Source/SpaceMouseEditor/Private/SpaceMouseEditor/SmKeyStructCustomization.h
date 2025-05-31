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

namespace SpaceMouse::Editor
{
	class SPACEMOUSEEDITOR_API FSmKeyStructCustomization : public IPropertyTypeCustomization
	{
	public:
		FSmKeyStructCustomization();
		~FSmKeyStructCustomization();
    
		virtual void CustomizeHeader(
			TSharedRef<IPropertyHandle> propertyHandle,
			FDetailWidgetRow& headerRow,
			IPropertyTypeCustomizationUtils& customizationUtils
		) override;
    
		virtual void CustomizeChildren(
			TSharedRef<IPropertyHandle> propertyHandle,
			IDetailChildrenBuilder& childBuilder,
			IPropertyTypeCustomizationUtils& customizationUtils
		) override { };
    
		virtual bool ShouldInlineKey() const override { return true; }
    
		void CustomizeHeaderOnlyWithButton(
			TSharedRef<IPropertyHandle> const& propertyHandle,
			FDetailWidgetRow& headerRow,
			IPropertyTypeCustomizationUtils& customizationUtils,
			TSharedRef<SWidget> const& button
		);
    
		static TSharedRef<IPropertyTypeCustomization> Make();

	protected:

		TOptional<FKey> GetCurrentKey() const;
		void OnKeyChanged(TSharedPtr<FKey> selectedKey);
		TSharedPtr<IPropertyHandle> PropertyHandle;
	};
}
