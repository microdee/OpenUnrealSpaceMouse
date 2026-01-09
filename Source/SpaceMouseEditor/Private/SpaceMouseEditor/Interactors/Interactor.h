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
#include "Mcro/Common.h"

namespace SpaceMouse::Editor::Interactor
{
	using namespace Mcro::Common;
	
	class IWidgetInteractionMode;

	class SPACEMOUSEEDITOR_API IWidgetInteractionContext
		: public TAutoModularFeature<IWidgetInteractionContext>
		, public IHaveType
	{
	protected:
		template <typename Self>
		void RegisterContext(this Self&& self)
		{
			self.Register();
			self.SetType();
		}
		TArray<TSharedPtr<IWidgetInteractionMode>> InteractionStack;

	public:
		FBool IsActive { false };
		virtual void Tick();

		virtual void PushInteraction(TSharedRef<IWidgetInteractionMode> const& interaction);
		virtual void SetInteraction(TSharedRef<IWidgetInteractionMode> const& interaction);
		virtual void PopInteraction(TSharedPtr<IWidgetInteractionMode> const& interaction = {});
	};
	
	class SPACEMOUSEEDITOR_API IWidgetInteractionMode : public IHaveType
	{
	protected:
		template <typename Self>
		void ConstructInteractor(this Self&& self)
		{
			self.SetType();
		}
		
	public:
		FBool IsActive { false };
		virtual void Tick(IWidgetInteractionContext& ctx) = 0;
		virtual bool IsValid(IWidgetInteractionContext& ctx) = 0;
	};
}
