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

#include "SpaceMouseConfig.h"


#include "DetailCategoryBuilder.h"
#include "DetailLayoutBuilder.h"
#include "DetailWidgetRow.h"
#include "Editor.h"
#include "EditorViewportCommands.h"
#include "ISettingsModule.h"
#include "SpaceMouseRuntime/SmInputDevice.h"
#include "Framework/Application/SlateApplication.h"
#include "Framework/Commands/InputBindingManager.h"
#include "Misc/MessageDialog.h"
#include "Modules/ModuleManager.h"
#include "Widgets/Input/SRichTextHyperlink.h"
#include "Widgets/Text/STextBlock.h"
#include "Logging/StructuredLog.h"
#include "ISettingsModule.h"
#include "ISettingsSection.h"
#include "ISettingsContainer.h"

#include "Mcro/Common.h"
#include "SpaceMouseEditor.h"

DECLARE_LOG_CATEGORY_CLASS(LogSpaceMouseConfig, Log, Log);

using namespace Mcro::Common;
using namespace SpaceMouse::Runtime;
using namespace SpaceMouse::Reader;

namespace
{
	[[maybe_unused]]
	TObserveModule<FSpaceMouseEditorModule> GObserveModule {{
		.OnStartup = []
		{
			using namespace Mcro::Common::InferDelegate;
			
			auto& propertyModule = LoadUnrealModule<FPropertyEditorModule>();
	    
			propertyModule.RegisterCustomClassLayout(
				NAME_"SpaceMouseConfig",
				From(&FSpaceMouseConfigCustomization::Make)
			);

			propertyModule.NotifyCustomizationModuleChanged();

			auto& settingsModule = LoadUnrealModule<ISettingsModule>();
			auto settings = GetMutableDefault<USpaceMouseConfig>();

			auto settingsSection = settingsModule.RegisterSettings(
				NAME_"Editor", NAME_"Plugins", NAME_"SpaceMouse",
				INVTEXT_"SpaceMouse",
				INVTEXT_"Configure SpaceMice for the editor",
				settings
			);

			if (settingsSection.IsValid())
				settingsSection->OnModified().BindLambda([]
				{
					auto settings = GetMutableDefault<USpaceMouseConfig>();
					settings->SaveConfig();
					return true;
				});

			settings->RegisterInputBindingNotification();
		},
		
		.OnShutdown = []
		{
			if (!UObjectInitialized()) return;
			if (auto propertyModule = GetUnrealModulePtr<FPropertyEditorModule>())
				propertyModule->UnregisterCustomClassLayout(NAME_"SpaceMouseConfig");
	    
			if (auto settingsModule = GetUnrealModulePtr<ISettingsModule>())
				settingsModule->UnregisterSettings(NAME_"Editor", NAME_"Plugins", NAME_"SpaceMouse");
		}
	}};
}

// Add default functionality here for any ISpaceMouseConfig functions that are not pure virtual.
USpaceMouseConfig::USpaceMouseConfig(const FObjectInitializer& oi) : Super(oi)
{
	TranslationCurve.GetRichCurve()->AddKey(0.0, 0.0);
	TranslationCurve.GetRichCurve()->AddKey(1.0, 1.0);
	RotationCurve.GetRichCurve()->AddKey(0.0, 0.0);
	RotationCurve.GetRichCurve()->AddKey(1.0, 1.0);

	IncreaseSpeedButton = FSmInputDevice::GetKeyFrom(Buttons::ECmd::KeyF2);
	DecreaseSpeedButton = FSmInputDevice::GetKeyFrom(Buttons::ECmd::KeyF1);
	ResetSpeedButton = FSmInputDevice::GetKeyFrom(Buttons::ECmd::KeyF3);
	ResetRollButton = FSmInputDevice::GetKeyFrom(Buttons::ECmd::FilterRotate);
	ShowSpaceMousePreferencesButton = FSmInputDevice::GetKeyFrom(Buttons::ECmd::MenuOptions);
	ShowInputBindingsButton = FSmInputDevice::GetKeyFrom(Buttons::ECmd::MenuButtonMappingEditor);
}

FSmUserSettings USpaceMouseConfig::GetUserSettings() const
{
	return
	{
		DisplayDebugInformation,
		MovementSecondsTolerance,
		{
			XTranslationAxisMap,
			YTranslationAxisMap,
			ZTranslationAxisMap,
			TranslationUnitsPerSec,
			*TranslationCurve.GetRichCurveConst()
		},
		{
			PitchAxisMap,
			YawAxisMap,
			RollAxisMap,
			RotationDegreesPerSec,
			*RotationCurve.GetRichCurveConst()
		}
	};
}

void USpaceMouseConfig::RegisterCustomInputBinding(FUICommandInfo const& cmd)
{
	auto key = TEXT_"{0}_{1}" _FMT(cmd.GetBindingContext(), cmd.GetCommandName()); 
	CustomInputBindings.Add(key, {cmd.GetBindingContext(), cmd.GetCommandName()});
	SaveConfig();
	FMT_LOG(LogSpaceMouseConfig, Display, "Input Binding {0} / {1} is handled", cmd.GetBindingContext(), cmd.GetCommandName());
}

void USpaceMouseConfig::HandleUserDefinedChordChanged(FUICommandInfo const& cmd)
{
	const auto primaryChord = cmd.GetActiveChord(EMultipleKeyBindingIndex::Primary);
	const auto secondaryChord = cmd.GetActiveChord(EMultipleKeyBindingIndex::Secondary);

	auto key = TEXT_"{0}_{1}" _FMT(cmd.GetBindingContext(), cmd.GetCommandName());
	
	if(FSmInputDevice::GetButtonFrom(primaryChord->Key) != Buttons::ECmd::Noop)
	{
		RegisterCustomInputBinding(cmd);
	}
	else if (FSmInputDevice::GetButtonFrom(secondaryChord->Key) != Buttons::ECmd::Noop)
	{
		RegisterCustomInputBinding(cmd);
	}
	else if(CustomInputBindings.Contains(key))
	{
		CustomInputBindings.Remove(key);
	}
}

void USpaceMouseConfig::RegisterInputBindingNotification()
{
	// Apparently the Input Binding config doesn't trigger SpaceMouse bindings,
	// which were not configured yet during the current runtime session,
	// despite the fact that it does keep the configuration FKey references to the SpaceMouse buttons.
	// Attempting to work around that, we keep the custom bindings in our own config
	// and remind the Input Binding system to kindly please consider our SpaceMouse bindings too.

	using namespace Mcro::Common::InferDelegate;

	GEditor->GetTimerManager().Get().SetTimerForNextTick(FTimerDelegate::CreateLambda([this]
	{
		auto& ibm = FInputBindingManager::Get();
		for(const auto& pair : CustomInputBindings)
		{
			const auto [context, command] = pair.Value;
			auto cmd = ibm.FindCommandInContext(context, command);
			ibm.NotifyActiveChordChanged(*cmd.Get(), EMultipleKeyBindingIndex::Primary);
			ibm.NotifyActiveChordChanged(*cmd.Get(), EMultipleKeyBindingIndex::Secondary);
		}

		ibm.SaveInputBindings();
		
		ibm.RegisterUserDefinedChordChanged(From(this, &USpaceMouseConfig::HandleUserDefinedChordChanged));
	}));
}

void USpaceMouseConfig::GoToSmConfig() const
{
	GetUnrealModule<ISettingsModule>()
		.ShowViewer(NAME_"Editor", NAME_"Plugins", NAME_"SpaceMouse");
}

void USpaceMouseConfig::GoToInputBindings() const
{
	GetUnrealModule<ISettingsModule>()
		.ShowViewer(NAME_"Editor", NAME_"General", NAME_"InputBindings");
}

void USpaceMouseConfig::SetDefaultBindings(bool askUser)
{
	using namespace SpaceMouse::Reader::Buttons;
	if(askUser)
	{
		auto dialogResult = FMessageDialog::Open(
			EAppMsgType::YesNo,
			INVTEXT_
				"This action might overwrite some pre-existing Secondary keyboard shortcuts."
				// TODO: list affected shortcuts
				"\nThis action will NOT overwrite any Primary editor keyboard shortcuts."
				"\nDo you want to continue?"
		);
		if(dialogResult != EAppReturnType::Yes) return;
	}

	// TODO: change it per model?

	auto self = GetMutableDefault<USpaceMouseConfig>();
	self->IncreaseSpeedButton = FSmInputDevice::GetKeyFrom(ECmd::KeyF2);
	self->DecreaseSpeedButton = FSmInputDevice::GetKeyFrom(ECmd::KeyF1);
	self->ResetSpeedButton = FSmInputDevice::GetKeyFrom(ECmd::KeyF3);
	self->ShowSpaceMousePreferencesButton = FSmInputDevice::GetKeyFrom(ECmd::MenuOptions);
	self->ShowInputBindingsButton = FSmInputDevice::GetKeyFrom(ECmd::MenuButtonMappingEditor);
	
	// transform meaning until per-device data is ready
	self->ResetRollButton = FSmInputDevice::GetKeyFrom(ECmd::FilterRotate);

	auto& ibm = FInputBindingManager::Get();
	auto& edVpCmds = FEditorViewportCommands::Get();
	
	SetCommandBinding(edVpCmds.Top, ECmd::ViewTop);
	SetCommandBinding(edVpCmds.Bottom, ECmd::ViewBottom);
	SetCommandBinding(edVpCmds.Front, ECmd::ViewFront);
	SetCommandBinding(edVpCmds.Back, ECmd::ViewBack);
	SetCommandBinding(edVpCmds.Right, ECmd::ViewRight);
	SetCommandBinding(edVpCmds.Left, ECmd::ViewLeft);
	SetCommandBinding(edVpCmds.FocusViewportToSelection, ECmd::ViewFit);

	// transform meaning until per-device data is ready
	SetCommandBinding(edVpCmds.Perspective, ECmd::ViewRollCW);

	ibm.SaveInputBindings();
}

void USpaceMouseConfig::SetCommandBinding(FInputBindingManager& ibm, FName const& cmdCtx, FName cmd, Buttons::ECmd button)
{
	auto cmdInfo = ibm.FindCommandInContext(cmdCtx, cmd);
	SetCommandBinding(cmdInfo, button);
}

void USpaceMouseConfig::SetCommandBinding(TSharedPtr<FUICommandInfo> const& cmd, Buttons::ECmd button)
{
	if(!cmd) return;
	cmd->SetActiveChord(FInputChord(FSmInputDevice::GetKeyFrom(button)), EMultipleKeyBindingIndex::Secondary);
}

TSharedRef<IDetailCustomization> FSpaceMouseConfigCustomization::Make()
{
	return MakeShared<FSpaceMouseConfigCustomization>();
}

void FSpaceMouseConfigCustomization::CustomizeDetails(IDetailLayoutBuilder& detailBuilder)
{
	using namespace Mcro::Common::InferDelegate;
	
	auto PropertyInButtonMapping = detailBuilder.GetProperty(
		GET_MEMBER_NAME_CHECKED(USpaceMouseConfig, ResetRollButton)
	);
	detailBuilder.AddCustomRowToCategory(
		PropertyInButtonMapping,
		INVTEXT_"Button Binding Notice"
	)
	. WholeRowContent()
	[
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		. AutoHeight()
		. HAlign(HAlign_Left)
		. VAlign(VAlign_Top)
		. Padding(0, 20, 0, 5)
		[
			SNew(STextBlock)
			. Text(INVTEXT_
				"Button bindings are now configurable via the 'Keyboard Shortcuts' section in Editor Preferences."
				"\nWhen setting an action mapping, just press the desired button on your device to bind it to the selected action."
			)
		]
		+ SVerticalBox::Slot()
		. AutoHeight()
		. HAlign(HAlign_Left)
		. VAlign(VAlign_Bottom)
		. Padding(0, 0, 0, 10)
		[
			SNew(SHyperlink)
			. Text(INVTEXT_"Go to Keyboard Shortcuts")
			. OnNavigate(From([]
			{
				GetMutableDefault<USpaceMouseConfig>()->GoToInputBindings();
			}))
		]
		+ SVerticalBox::Slot()
		. AutoHeight()
		. HAlign(HAlign_Left)
		. VAlign(VAlign_Bottom)
		. Padding(0, 0, 0, 20)
		[
			SNew(SHyperlink)
			. Text(INVTEXT_"Configure default button bindings")
			. ToolTipText(INVTEXT_"This doesn't take the connected device model into account yet. Soon (TM)")
			. OnNavigate(From([]
			{
				USpaceMouseConfig::SetDefaultBindings(true);
			}))
		]
	];
}
