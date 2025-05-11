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
#include "SmInputDevice.h"
#include "Framework/Application/SlateApplication.h"
#include "Framework/Commands/InputBindingManager.h"
#include "Misc/MessageDialog.h"
#include "Modules/ModuleManager.h"
#include "Widgets/Input/SRichTextHyperlink.h"
#include "Widgets/Text/STextBlock.h"

#define LOCTEXT_NAMESPACE "USpaceMouseConfig"

// Add default functionality here for any ISpaceMouseConfig functions that are not pure virtual.
USpaceMouseConfig::USpaceMouseConfig(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    TranslationCurve.GetRichCurve()->AddKey(0.0, 0.0);
    TranslationCurve.GetRichCurve()->AddKey(1.0, 1.0);
    RotationCurve.GetRichCurve()->AddKey(0.0, 0.0);
    RotationCurve.GetRichCurve()->AddKey(1.0, 1.0);

    IncreaseSpeedButton = FSmInputDevice::GetKeyFrom(EV3DCmd::KeyF2);
    DecreaseSpeedButton = FSmInputDevice::GetKeyFrom(EV3DCmd::KeyF1);
    ResetSpeedButton = FSmInputDevice::GetKeyFrom(EV3DCmd::KeyF3);
    ResetRollButton = FSmInputDevice::GetKeyFrom(EV3DCmd::FilterRotate);
    ShowSpaceMousePreferencesButton = FSmInputDevice::GetKeyFrom(EV3DCmd::MenuOptions);
    ShowInputBindingsButton = FSmInputDevice::GetKeyFrom(EV3DCmd::MenuButtonMappingEditor);
}

FSmUserSettings USpaceMouseConfig::GetUserSettings()
{
    return
    {
        DisplayDebugInformation,
        MaxHidReadOperationsPerFrame,
        MovementSecondsTolerance,
        {
            XTranslationAxisMap,
            YTranslationAxisMap,
            ZTranslationAxisMap,
            TranslationUnitsPerSec,
            TranslationCurve.GetRichCurve()
        },
        {
            PitchAxisMap,
            YawAxisMap,
            RollAxisMap,
            RotationDegreesPerSec,
            RotationCurve.GetRichCurve()
        }
    };
}

void USpaceMouseConfig::RegisterCustomInputBinding(const FUICommandInfo& Cmd)
{
    const FString Key = Cmd.GetBindingContext().ToString() + TEXT("_") + Cmd.GetCommandName().ToString(); 
    CustomInputBindings.Add(Key, {Cmd.GetBindingContext(), Cmd.GetCommandName()});
    SaveConfig();
    UE_LOG(LogTemp, Display, TEXT("Input Binding %s / %s is handled"), *Cmd.GetBindingContext().ToString(), *Cmd.GetCommandName().ToString());
}

void USpaceMouseConfig::HandleUserDefinedChordChanged(const FUICommandInfo& Cmd)
{
    const auto PrimChord = Cmd.GetActiveChord(EMultipleKeyBindingIndex::Primary);
    const auto SecChord = Cmd.GetActiveChord(EMultipleKeyBindingIndex::Secondary);

    const FString Key = Cmd.GetBindingContext().ToString() + TEXT("_") + Cmd.GetCommandName().ToString();
    
    if(FSmInputDevice::GetButtonFrom(PrimChord->Key) != EV3DCmd::Noop)
    {
        RegisterCustomInputBinding(Cmd);
    }
    else if (FSmInputDevice::GetButtonFrom(SecChord->Key) != EV3DCmd::Noop)
    {
        RegisterCustomInputBinding(Cmd);
    }
    else if(CustomInputBindings.Contains(Key))
    {
        CustomInputBindings.Remove(Key);
    }
}

void USpaceMouseConfig::RegisterInputBindingNotification()
{
    // Apparently the Input Binding config doesn't trigger SpaceMouse bindings,
    // which were not configured yet during the current runtime session,
    // despite the fact that it does keep the configuration FKey references to the SpaceMouse buttons.
    // Attempting to work around that, we keep the custom bindings in our own config
    // and remind the Input Binding system to kindly please consider our SpaceMouse bindings too.

    GEditor->GetTimerManager().Get().SetTimerForNextTick(FTimerDelegate::CreateLambda([this]()
    {
        auto& Ibm = FInputBindingManager::Get();
        for(const auto& Kvp : CustomInputBindings)
        {
            const auto [Context, Command] = Kvp.Value;
            auto Cmd = Ibm.FindCommandInContext(Context, Command);
            Ibm.NotifyActiveChordChanged(*Cmd.Get(), EMultipleKeyBindingIndex::Primary);
            Ibm.NotifyActiveChordChanged(*Cmd.Get(), EMultipleKeyBindingIndex::Secondary);
        }

        Ibm.SaveInputBindings();
        
        Ibm.RegisterUserDefinedChordChanged(
            FOnUserDefinedChordChanged::FDelegate::CreateUObject(this, &USpaceMouseConfig::HandleUserDefinedChordChanged)
        );
    }));
}

void USpaceMouseConfig::GoToSmConfig() const
{
    ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");
    SettingsModule->ShowViewer("Editor", "Plugins", "SpaceMouse");
}

void USpaceMouseConfig::GoToInputBindings() const
{
    ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");
    SettingsModule->ShowViewer("Editor", "General", "InputBindings");
}

void USpaceMouseConfig::SetDefaultBindings(bool bAskUser)
{
    if(bAskUser)
    {
        auto DialogResult = FMessageDialog::Open(
            EAppMsgType::YesNo,
            LOCTEXT(
                "SmConfig_ConfirmSettingDefaultBindings",
                "This action might overwrite some pre-existing Secondary keyboard shortcuts.\n"
                // TODO: list affected shortcuts
                "This action will NOT overwrite any Primary editor keyboard shortcuts.\n"
                "Do you want to continue?"
            )
        );
        if(DialogResult != EAppReturnType::Yes)
        {
            return;
        }
    }

    // TODO: change it per model?

    auto Self = GetMutableDefault<USpaceMouseConfig>();
    Self->IncreaseSpeedButton = FSmInputDevice::GetKeyFrom(EV3DCmd::KeyF2);
    Self->DecreaseSpeedButton = FSmInputDevice::GetKeyFrom(EV3DCmd::KeyF1);
    Self->ResetSpeedButton = FSmInputDevice::GetKeyFrom(EV3DCmd::KeyF3);
    Self->ShowSpaceMousePreferencesButton = FSmInputDevice::GetKeyFrom(EV3DCmd::MenuOptions);
    Self->ShowInputBindingsButton = FSmInputDevice::GetKeyFrom(EV3DCmd::MenuButtonMappingEditor);
    
    // transform meaning until per-device data is ready
    Self->ResetRollButton = FSmInputDevice::GetKeyFrom(EV3DCmd::FilterRotate);

    auto& Ibm = FInputBindingManager::Get();
    auto& EdVpCmds = FEditorViewportCommands::Get();
    
    SetCommandBinding(EdVpCmds.Top, EV3DCmd::ViewTop);
    SetCommandBinding(EdVpCmds.Bottom, EV3DCmd::ViewBottom);
    SetCommandBinding(EdVpCmds.Front, EV3DCmd::ViewFront);
    SetCommandBinding(EdVpCmds.Back, EV3DCmd::ViewBack);
    SetCommandBinding(EdVpCmds.Right, EV3DCmd::ViewRight);
    SetCommandBinding(EdVpCmds.Left, EV3DCmd::ViewLeft);
    SetCommandBinding(EdVpCmds.FocusViewportToSelection, EV3DCmd::ViewFit);

    // transform meaning until per-device data is ready
    SetCommandBinding(EdVpCmds.Perspective, EV3DCmd::ViewRollCW);

    Ibm.SaveInputBindings();
}

void USpaceMouseConfig::SetCommandBinding(FInputBindingManager& Ibm, FName InCmdCtx, FName InCmd, EV3DCmd SmButton)
{
    auto Cmd = Ibm.FindCommandInContext(InCmdCtx, InCmd);
    SetCommandBinding(Cmd, SmButton);
}

void USpaceMouseConfig::SetCommandBinding(TSharedPtr<FUICommandInfo> InCmd, EV3DCmd SmButton)
{
    if(!InCmd) return;
    InCmd->SetActiveChord(FInputChord(FSmInputDevice::GetKeyFrom(SmButton)), EMultipleKeyBindingIndex::Secondary);
}

TSharedRef<IDetailCustomization> FSpaceMouseConfigCustomization::MakeInstance()
{
    return MakeShared<FSpaceMouseConfigCustomization>();
}

void FSpaceMouseConfigCustomization::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
    auto PropertyInButtonMapping = DetailBuilder.GetProperty(
        GET_MEMBER_NAME_CHECKED(USpaceMouseConfig, ResetRollButton)
    );
    DetailBuilder.AddCustomRowToCategory(
        PropertyInButtonMapping,
        LOCTEXT("SmConfig_ButtonBindingNotice", "Button Binding Notice")
    )
    . WholeRowContent()
    [
        SNew(SVerticalBox)
        + SVerticalBox::Slot()
        . AutoHeight()
        . HAlign(EHorizontalAlignment::HAlign_Left)
        . VAlign(EVerticalAlignment::VAlign_Top)
        . Padding(0, 20, 0, 5)
        [
            SNew(STextBlock)
            . Text(LOCTEXT(
                "SmConfig_ButtonBindingNotice_Text",
                "Button bindings are now configurable via the 'Keyboard Shortcuts' section in Editor Preferences.\n"
                "When setting an action mapping, just press the desired button on your device to bind it to the selected action."
            ))
        ]
        + SVerticalBox::Slot()
        . AutoHeight()
        . HAlign(EHorizontalAlignment::HAlign_Left)
        . VAlign(EVerticalAlignment::VAlign_Bottom)
        . Padding(0, 0, 0, 10)
        [
            SNew(SHyperlink)
            . Text(LOCTEXT(
                "SmConfig_GoToKeyboardShortcuts",
                "Go to Keyboard Shortcuts"
            ))
            . OnNavigate(FSimpleDelegate::CreateLambda([]()
            {
                GetMutableDefault<USpaceMouseConfig>()->GoToInputBindings();
            }))
        ]
        + SVerticalBox::Slot()
        . AutoHeight()
        . HAlign(EHorizontalAlignment::HAlign_Left)
        . VAlign(EVerticalAlignment::VAlign_Bottom)
        . Padding(0, 0, 0, 20)
        [
            SNew(SHyperlink)
            . Text(LOCTEXT(
                "SmConfig_PreconfigureBindings",
                "Configure default button bindings"
            ))
            . ToolTipText(LOCTEXT(
                "SmConfig_PreconfigureBindingsTooltip",
                "This doesn't take the connected device model into account yet (WIP)"
            ))
            . OnNavigate(FSimpleDelegate::CreateLambda([]()
            {
                USpaceMouseConfig::SetDefaultBindings(true);
            }))
        ]
    ];
}

#undef LOCTEXT_NAMESPACE
