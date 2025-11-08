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
	
	class IWidgetInteraction;

	class SPACEMOUSEEDITOR_API IInteractionContext
		: public TAutoModularFeature<IInteractionContext>
		, public IHaveType
	{
	protected:
		template <typename Self>
		void RegisterContext(this Self&& self)
		{
			self.Register();
			self.SetType();
		}

		TArray<IWidgetInteraction*> InteractionStack;

	public:
		virtual void Tick() = 0;
		TEventDelegate<void(IInteractionContext* ctx)> OnContextFocus;
		virtual void BeginContext() {};
		virtual void EndContext() {};

		virtual void PushInteraction(IWidgetInteraction* interaction);
		virtual void PopInteraction(IWidgetInteraction* interaction);
	};
	
	class SPACEMOUSEEDITOR_API IWidgetInteraction
		: public TAutoModularFeature<IWidgetInteraction>
		, public IHaveType
	{
	protected:
		template <typename Self>
		void RegisterInteractor(this Self&& self)
		{
			self.Register();
			self.SetType();
		}
		
	public:
		virtual FName GetName() const = 0;
		virtual FString GetDisplayName() const = 0;
		virtual bool SupportsContext(FType const& context) = 0;
		virtual bool IsSupported(IInteractionContext& ctx) = 0;
		virtual bool IsEnforced(IInteractionContext& ctx) = 0;
		virtual FString UserSelectionCategory() { return {}; };
	};
}
