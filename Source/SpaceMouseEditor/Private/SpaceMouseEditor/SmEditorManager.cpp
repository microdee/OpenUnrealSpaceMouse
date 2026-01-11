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



#include "SpaceMouseEditor/SmEditorManager.h"
#include "SpaceMouseEditor/SmViewportOverlay.h"
#include "SpaceMouseEditor.h"
#include "SpaceMouseRuntime/SmInputDevice.h"
#include "SpaceMouseReader/MovementState.h"
#include "CommonBehaviors.h"

#include "CameraController.h"
#include "Editor.h"
#include "GameFramework/PlayerInput.h"
#include "SEditorViewport.h"
#include "EditorViewportClient.h"
#include "SpaceMouseConfig.h"
#include "HAL/PlatformApplicationMisc.h"
#include "Framework/Application/SlateApplication.h"
#include "Interactors/Interactor.h"

namespace SpaceMouse::Editor
{
	using namespace SpaceMouse::Runtime;

	TModuleBoundObject<FSpaceMouseEditorModule, FSmEditorManager> GEditorManager {};
	
	void FSmEditorManager::Tick(float DeltaSecs)
	{
		using namespace SpaceMouse::Editor::Interactor;

		TickManager(DeltaSecs);

		if (ShouldInitialize)
		{
			auto& ibm = FInputBindingManager::Get();
			ibm.SaveInputBindings();

			for (IWidgetInteractionContext* ctx : IWidgetInteractionContext::GetAll())
			{
				ctx->IsActive.OnChange(this, [this, ctx](bool isActive)
				{
					CurrentContext = ctx;
					if (isActive)
					{
						for (IWidgetInteractionContext* other : IWidgetInteractionContext::GetAll())
						{
							if (other == ctx) continue;
							other->IsActive = false;
						}
					}
				});
			}
		}

		auto settings = GetMutableDefault<USpaceMouseConfig>();
		IsAppForeground = settings->ActiveInBackground || FPlatformApplicationMisc::IsThisApplicationForeground();
		IsActive = !Learning && IsAppForeground;
		if (IsActive.OnUp())
		{
			for (IWidgetInteractionContext* ctx : IWidgetInteractionContext::GetAll())
			{
				ctx->IsActive = false;
			}
		}
		if (IsActive.OnDown() && CurrentContext)
		{
			CurrentContext->IsActive = true;
		}

		for (IWidgetInteractionContext* ctx : IWidgetInteractionContext::GetAll())
		{
			ctx->Tick();
		}

		TriggerCustomButtons();
	}

	void FSmEditorManager::TriggerCustomButtons()
	{
		auto settings = GetMutableDefault<USpaceMouseConfig>();

		if (IsActive)
		{
			if (GetButton(FSmInputDevice::GetButtonFrom(settings->ShowSpaceMousePreferencesButton)).OnDown())
				settings->GoToSmConfig();
			
			if (GetButton(FSmInputDevice::GetButtonFrom(settings->ShowInputBindingsButton)).OnDown())
				settings->GoToInputBindings();
		}
	}

	FSmUserSettings FSmEditorManager::GetUserSettings()
	{
		return GetMutableDefault<USpaceMouseConfig>()->GetUserSettings();
	}

	FSmEditorManager& FSmEditorManager::Get()
	{
		return GEditorManager.GetChecked();
	}

	void FSmEditorManager::BeginLearning()
	{
		Learning = true;
	}

	void FSmEditorManager::EndLearning()
	{
		Learning = false;
	}
}
