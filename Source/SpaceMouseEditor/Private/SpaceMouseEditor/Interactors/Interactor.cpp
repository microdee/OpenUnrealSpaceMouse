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

#include "Interactor.h"

namespace SpaceMouse::Editor::Interactor
{
	void IWidgetInteractionContext::Tick()
	{
		bool isTop = true;
		for (int i = InteractionStack.Num() - 1; i >= 0; --i)
		{
			if (auto interaction = InteractionStack[i])
			{
				if (interaction->IsValid(*this))
				{
					interaction->IsActive = isTop && IsActive;
					isTop = false;
					if (interaction->IsActive)
					{
						interaction->Tick(*this);
					}
				}
				else
				{
					interaction->IsActive = false;
				}
			}
			else
			{
				InteractionStack.Pop();
			}
		}
	}

	void IWidgetInteractionContext::PushInteraction(TSharedRef<IWidgetInteractionMode> const& interaction)
	{
		InteractionStack.Push(interaction);
	}

	void IWidgetInteractionContext::PopInteraction(TSharedPtr<IWidgetInteractionMode> const& interaction)
	{
		if (interaction)
		{
			for (int i = InteractionStack.Num() - 1; i >= 0; --i)
			{
				if (InteractionStack[i].Get() == interaction.Get())
				{
					InteractionStack.RemoveAt(i);
					return;
				}
			}
		}
		else
		{
			InteractionStack.Pop();
		}
	}

	void IWidgetInteractionContext::SetInteraction(TSharedRef<IWidgetInteractionMode> const& interaction)
	{
		InteractionStack.Empty(1);
		InteractionStack.Add(interaction);
	}
}
