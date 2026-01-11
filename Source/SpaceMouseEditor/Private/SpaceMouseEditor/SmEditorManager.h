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

#include "SpaceMouseReader/Manager.h"
#include "Misc/App.h"
#include "Input/Events.h"
#include "TimerManager.h"

struct FInputActionKeyMapping;
class FEditorViewportClient;

namespace SpaceMouse::Editor
{
	namespace Interactor
	{
		class IWidgetInteractionContext;
	}

	using namespace SpaceMouse::Reader;
	
	class FSmViewportOverlay;
	
	class SPACEMOUSEEDITOR_API FSmEditorManager
		: public IManager
		, public FTickableGameObject
	{
	private:
		FOnce ShouldInitialize;

		FBool Learning { false };
		FBool IsAppForeground { false };
		FBool IsActive { false };

		Interactor::IWidgetInteractionContext* CurrentContext = nullptr;

	protected:
		virtual FSmUserSettings GetUserSettings() override;

	public:

		static FSmEditorManager& Get();

		void BeginLearning();
		void EndLearning();
		bool IsLearning() const { return Learning; }

		virtual void Tick(float DeltaSecs) override;
		virtual bool IsTickableWhenPaused() const override { return true; }
		virtual bool IsTickableInEditor() const override { return true; }
		virtual TStatId GetStatId() const override { return {}; }

		void TriggerCustomButtons();
	};
}
