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




#include "SmButtons.h"

EV3DCmd FButtonIterator::operator*() const
{
    return Button;
}

FButtonIterator& FButtonIterator::operator++()
{
    Button = FSmButton::FromID(FSmButton::FromCmd(Button) + 1);
    return *this;
}

FButtonIterator& FButtonIterator::operator--()
{
    Button = FSmButton::FromID(FSmButton::FromCmd(Button) + 1);
    return *this;
}

bool FButtonIterator::operator==(const FButtonIterator& Other) const
{
    return Button == Other.Button;
}

bool FButtonIterator::operator!=(const FButtonIterator& Other) const
{
    return  Button != Other.Button;
}

// ReSharper disable once CppMemberFunctionMayBeStatic
// ReSharper disable once CppUE4CodingStandardNamingViolationWarning
FButtonIterator FAllSmButtons::begin()
{
    return { EV3DCmd::MenuOptions };
}

// ReSharper disable once CppMemberFunctionMayBeStatic
// ReSharper disable once CppUE4CodingStandardNamingViolationWarning
FButtonIterator FAllSmButtons::end()
{
    return { EV3DCmd::COUNT };
}

EV3DCmd FSmButton::FromID(int ButtonID)
{
    return static_cast<EV3DCmd>(ButtonID);
}

int FSmButton::FromCmd(EV3DCmd CmdCode)
{
    return static_cast<int>(CmdCode);
}

FName FSmButton::GetNameOf(EV3DCmd Button)
{
    switch (Button)
    {
        case EV3DCmd::MenuOptions:                      return TEXT("MenuOptions");
        case EV3DCmd::ViewFit:                          return TEXT("ViewFit");
        case EV3DCmd::ViewTop:                          return TEXT("ViewTop");
        case EV3DCmd::ViewLeft:                         return TEXT("ViewLeft");
        case EV3DCmd::ViewRight:                        return TEXT("ViewRight");
        case EV3DCmd::ViewFront:                        return TEXT("ViewFront");
        case EV3DCmd::ViewBottom:                       return TEXT("ViewBottom");
        case EV3DCmd::ViewBack:                         return TEXT("ViewBack");
        case EV3DCmd::ViewRollCW:                       return TEXT("ViewRollCW");
        case EV3DCmd::ViewRollCCW:                      return TEXT("ViewRollCCW");
        case EV3DCmd::ViewIso1:                         return TEXT("ViewIso1");
        case EV3DCmd::ViewIso2:                         return TEXT("ViewIso2");
        case EV3DCmd::KeyF1:                            return TEXT("KeyF1");
        case EV3DCmd::KeyF2:                            return TEXT("KeyF2");
        case EV3DCmd::KeyF3:                            return TEXT("KeyF3");
        case EV3DCmd::KeyF4:                            return TEXT("KeyF4");
        case EV3DCmd::KeyF5:                            return TEXT("KeyF5");
        case EV3DCmd::KeyF6:                            return TEXT("KeyF6");
        case EV3DCmd::KeyF7:                            return TEXT("KeyF7");
        case EV3DCmd::KeyF8:                            return TEXT("KeyF8");
        case EV3DCmd::KeyF9:                            return TEXT("KeyF9");
        case EV3DCmd::KeyF10:                           return TEXT("KeyF10");
        case EV3DCmd::KeyF11:                           return TEXT("KeyF11");
        case EV3DCmd::KeyF12:                           return TEXT("KeyF12");
        case EV3DCmd::KeyEsc:                           return TEXT("KeyEsc");
        case EV3DCmd::KeyAlt:                           return TEXT("KeyAlt");
        case EV3DCmd::KeyShift:                         return TEXT("KeyShift");
        case EV3DCmd::KeyCtrl:                          return TEXT("KeyCtrl");
        case EV3DCmd::FilterRotate:                     return TEXT("FilterRotate");
        case EV3DCmd::FilterPanzoom:                    return TEXT("FilterPanzoom");
        case EV3DCmd::FilterDominant:                   return TEXT("FilterDominant");
        case EV3DCmd::ScalePlus:                        return TEXT("ScalePlus");
        case EV3DCmd::ScaleMinus:                       return TEXT("ScaleMinus");
        case EV3DCmd::ViewSpinCW:                       return TEXT("ViewSpinCW");
        case EV3DCmd::ViewSpinCCW:                      return TEXT("ViewSpinCCW");
        case EV3DCmd::ViewTiltCW:                       return TEXT("ViewTiltCW");
        case EV3DCmd::ViewTiltCCW:                      return TEXT("ViewTiltCCW");
        case EV3DCmd::MenuPopup:                        return TEXT("MenuPopup");
        case EV3DCmd::MenuButtonMappingEditor:          return TEXT("MenuButtonMappingEditor");
        case EV3DCmd::MenuAdvancedSettingsEditor:       return TEXT("MenuAdvancedSettingsEditor");
        case EV3DCmd::MotionMacroZoom:                  return TEXT("MotionMacroZoom");
        case EV3DCmd::MotionMacroZoomOutCursorToCenter: return TEXT("MotionMacroZoomOutCursorToCenter");
        case EV3DCmd::MotionMacroZoomInCursorToCenter:  return TEXT("MotionMacroZoomInCursorToCenter");
        case EV3DCmd::MotionMacroZoomOutCenterToCenter: return TEXT("MotionMacroZoomOutCenterToCenter");
        case EV3DCmd::MotionMacroZoomInCenterToCenter:  return TEXT("MotionMacroZoomInCenterToCenter");
        case EV3DCmd::MotionMacroZoomOutCursorToCursor: return TEXT("MotionMacroZoomOutCursorToCursor");
        case EV3DCmd::MotionMacroZoomInCursorToCursor:  return TEXT("MotionMacroZoomInCursorToCursor");
        case EV3DCmd::ViewQzIn:                         return TEXT("ViewQzIn");
        case EV3DCmd::ViewQzOut:                        return TEXT("ViewQzOut");
        case EV3DCmd::KeyEnter:                         return TEXT("KeyEnter");
        case EV3DCmd::KeyDelete:                        return TEXT("KeyDelete");
        case EV3DCmd::KeyF13:                           return TEXT("KeyF13");
        case EV3DCmd::KeyF14:                           return TEXT("KeyF14");
        case EV3DCmd::KeyF15:                           return TEXT("KeyF15");
        case EV3DCmd::KeyF16:                           return TEXT("KeyF16");
        case EV3DCmd::KeyF17:                           return TEXT("KeyF17");
        case EV3DCmd::KeyF18:                           return TEXT("KeyF18");
        case EV3DCmd::KeyF19:                           return TEXT("KeyF19");
        case EV3DCmd::KeyF20:                           return TEXT("KeyF20");
        case EV3DCmd::KeyF21:                           return TEXT("KeyF21");
        case EV3DCmd::KeyF22:                           return TEXT("KeyF22");
        case EV3DCmd::KeyF23:                           return TEXT("KeyF23");
        case EV3DCmd::KeyF24:                           return TEXT("KeyF24");
        case EV3DCmd::KeyF25:                           return TEXT("KeyF25");
        case EV3DCmd::KeyF26:                           return TEXT("KeyF26");
        case EV3DCmd::KeyF27:                           return TEXT("KeyF27");
        case EV3DCmd::KeyF28:                           return TEXT("KeyF28");
        case EV3DCmd::KeyF29:                           return TEXT("KeyF29");
        case EV3DCmd::KeyF30:                           return TEXT("KeyF30");
        case EV3DCmd::KeyF31:                           return TEXT("KeyF31");
        case EV3DCmd::KeyF32:                           return TEXT("KeyF32");
        case EV3DCmd::KeyF33:                           return TEXT("KeyF33");
        case EV3DCmd::KeyF34:                           return TEXT("KeyF34");
        case EV3DCmd::KeyF35:                           return TEXT("KeyF35");
        case EV3DCmd::KeyF36:                           return TEXT("KeyF36");
        case EV3DCmd::View1:                            return TEXT("View1");
        case EV3DCmd::View2:                            return TEXT("View2");
        case EV3DCmd::View3:                            return TEXT("View3");
        case EV3DCmd::View4:                            return TEXT("View4");
        case EV3DCmd::View5:                            return TEXT("View5");
        case EV3DCmd::View6:                            return TEXT("View6");
        case EV3DCmd::View7:                            return TEXT("View7");
        case EV3DCmd::View8:                            return TEXT("View8");
        case EV3DCmd::View9:                            return TEXT("View9");
        case EV3DCmd::View10:                           return TEXT("View10");
        case EV3DCmd::View11:                           return TEXT("View11");
        case EV3DCmd::View12:                           return TEXT("View12");
        case EV3DCmd::View13:                           return TEXT("View13");
        case EV3DCmd::View14:                           return TEXT("View14");
        case EV3DCmd::View15:                           return TEXT("View15");
        case EV3DCmd::View16:                           return TEXT("View16");
        case EV3DCmd::View17:                           return TEXT("View17");
        case EV3DCmd::View18:                           return TEXT("View18");
        case EV3DCmd::View19:                           return TEXT("View19");
        case EV3DCmd::View20:                           return TEXT("View20");
        case EV3DCmd::View21:                           return TEXT("View21");
        case EV3DCmd::View22:                           return TEXT("View22");
        case EV3DCmd::View23:                           return TEXT("View23");
        case EV3DCmd::View24:                           return TEXT("View24");
        case EV3DCmd::View25:                           return TEXT("View25");
        case EV3DCmd::View26:                           return TEXT("View26");
        case EV3DCmd::View27:                           return TEXT("View27");
        case EV3DCmd::View28:                           return TEXT("View28");
        case EV3DCmd::View29:                           return TEXT("View29");
        case EV3DCmd::View30:                           return TEXT("View30");
        case EV3DCmd::View31:                           return TEXT("View31");
        case EV3DCmd::View32:                           return TEXT("View32");
        case EV3DCmd::View33:                           return TEXT("View33");
        case EV3DCmd::View34:                           return TEXT("View34");
        case EV3DCmd::View35:                           return TEXT("View35");
        case EV3DCmd::View36:                           return TEXT("View36");
        case EV3DCmd::SaveView1:                        return TEXT("SaveView1");
        case EV3DCmd::SaveView2:                        return TEXT("SaveView2");
        case EV3DCmd::SaveView3:                        return TEXT("SaveView3");
        case EV3DCmd::SaveView4:                        return TEXT("SaveView4");
        case EV3DCmd::SaveView5:                        return TEXT("SaveView5");
        case EV3DCmd::SaveView6:                        return TEXT("SaveView6");
        case EV3DCmd::SaveView7:                        return TEXT("SaveView7");
        case EV3DCmd::SaveView8:                        return TEXT("SaveView8");
        case EV3DCmd::SaveView9:                        return TEXT("SaveView9");
        case EV3DCmd::SaveView10:                       return TEXT("SaveView10");
        case EV3DCmd::SaveView11:                       return TEXT("SaveView11");
        case EV3DCmd::SaveView12:                       return TEXT("SaveView12");
        case EV3DCmd::SaveView13:                       return TEXT("SaveView13");
        case EV3DCmd::SaveView14:                       return TEXT("SaveView14");
        case EV3DCmd::SaveView15:                       return TEXT("SaveView15");
        case EV3DCmd::SaveView16:                       return TEXT("SaveView16");
        case EV3DCmd::SaveView17:                       return TEXT("SaveView17");
        case EV3DCmd::SaveView18:                       return TEXT("SaveView18");
        case EV3DCmd::SaveView19:                       return TEXT("SaveView19");
        case EV3DCmd::SaveView20:                       return TEXT("SaveView20");
        case EV3DCmd::SaveView21:                       return TEXT("SaveView21");
        case EV3DCmd::SaveView22:                       return TEXT("SaveView22");
        case EV3DCmd::SaveView23:                       return TEXT("SaveView23");
        case EV3DCmd::SaveView24:                       return TEXT("SaveView24");
        case EV3DCmd::SaveView25:                       return TEXT("SaveView25");
        case EV3DCmd::SaveView26:                       return TEXT("SaveView26");
        case EV3DCmd::SaveView27:                       return TEXT("SaveView27");
        case EV3DCmd::SaveView28:                       return TEXT("SaveView28");
        case EV3DCmd::SaveView29:                       return TEXT("SaveView29");
        case EV3DCmd::SaveView30:                       return TEXT("SaveView30");
        case EV3DCmd::SaveView31:                       return TEXT("SaveView31");
        case EV3DCmd::SaveView32:                       return TEXT("SaveView32");
        case EV3DCmd::SaveView33:                       return TEXT("SaveView33");
        case EV3DCmd::SaveView34:                       return TEXT("SaveView34");
        case EV3DCmd::SaveView35:                       return TEXT("SaveView35");
        case EV3DCmd::SaveView36:                       return TEXT("SaveView36");
        case EV3DCmd::KeyTab:                           return TEXT("KeyTab");
        case EV3DCmd::KeySpace:                         return TEXT("KeySpace");
        case EV3DCmd::Menu1:                            return TEXT("Menu1");
        case EV3DCmd::Menu2:                            return TEXT("Menu2");
        case EV3DCmd::Menu3:                            return TEXT("Menu3");
        case EV3DCmd::Menu4:                            return TEXT("Menu4");
        case EV3DCmd::Menu5:                            return TEXT("Menu5");
        case EV3DCmd::Menu6:                            return TEXT("Menu6");
        case EV3DCmd::Menu7:                            return TEXT("Menu7");
        case EV3DCmd::Menu8:                            return TEXT("Menu8");
        case EV3DCmd::Menu9:                            return TEXT("Menu9");
        case EV3DCmd::Menu10:                           return TEXT("Menu10");
        case EV3DCmd::Menu11:                           return TEXT("Menu11");
        case EV3DCmd::Menu12:                           return TEXT("Menu12");
        case EV3DCmd::Menu13:                           return TEXT("Menu13");
        case EV3DCmd::Menu14:                           return TEXT("Menu14");
        case EV3DCmd::Menu15:                           return TEXT("Menu15");
        case EV3DCmd::Menu16:                           return TEXT("Menu16");
        case EV3DCmd::Noop:                             return TEXT("Noop");
        default: return TEXT("Invalid");
    }
}

FString FSmButton::GetFriendlyNameOf(EV3DCmd Button)
{
    switch (Button)
    {
        case EV3DCmd::MenuOptions:                      return TEXT("Menu Options");
        case EV3DCmd::ViewFit:                          return TEXT("View Fit");
        case EV3DCmd::ViewTop:                          return TEXT("View Top");
        case EV3DCmd::ViewLeft:                         return TEXT("View Left");
        case EV3DCmd::ViewRight:                        return TEXT("View Right");
        case EV3DCmd::ViewFront:                        return TEXT("View Front");
        case EV3DCmd::ViewBottom:                       return TEXT("View Bottom");
        case EV3DCmd::ViewBack:                         return TEXT("View Back");
        case EV3DCmd::ViewRollCW:                       return TEXT("View Roll Cw");
        case EV3DCmd::ViewRollCCW:                      return TEXT("View Roll Ccw");
        case EV3DCmd::ViewIso1:                         return TEXT("View Iso 1");
        case EV3DCmd::ViewIso2:                         return TEXT("View Iso 2");
        case EV3DCmd::KeyF1:                            return TEXT("Key F1");
        case EV3DCmd::KeyF2:                            return TEXT("Key F2");
        case EV3DCmd::KeyF3:                            return TEXT("Key F3");
        case EV3DCmd::KeyF4:                            return TEXT("Key F4");
        case EV3DCmd::KeyF5:                            return TEXT("Key F5");
        case EV3DCmd::KeyF6:                            return TEXT("Key F6");
        case EV3DCmd::KeyF7:                            return TEXT("Key F7");
        case EV3DCmd::KeyF8:                            return TEXT("Key F8");
        case EV3DCmd::KeyF9:                            return TEXT("Key F9");
        case EV3DCmd::KeyF10:                           return TEXT("Key F10");
        case EV3DCmd::KeyF11:                           return TEXT("Key F11");
        case EV3DCmd::KeyF12:                           return TEXT("Key F12");
        case EV3DCmd::KeyEsc:                           return TEXT("Key Esc");
        case EV3DCmd::KeyAlt:                           return TEXT("Key Alt");
        case EV3DCmd::KeyShift:                         return TEXT("Key Shift");
        case EV3DCmd::KeyCtrl:                          return TEXT("Key Ctrl");
        case EV3DCmd::FilterRotate:                     return TEXT("Filter Rotate");
        case EV3DCmd::FilterPanzoom:                    return TEXT("Filter Panzoom");
        case EV3DCmd::FilterDominant:                   return TEXT("Filter Dominant");
        case EV3DCmd::ScalePlus:                        return TEXT("Scale Plus");
        case EV3DCmd::ScaleMinus:                       return TEXT("Scale Minus");
        case EV3DCmd::ViewSpinCW:                       return TEXT("View Spin CW");
        case EV3DCmd::ViewSpinCCW:                      return TEXT("View Spin CCW");
        case EV3DCmd::ViewTiltCW:                       return TEXT("View Tilt CW");
        case EV3DCmd::ViewTiltCCW:                      return TEXT("View Tilt CCW");
        case EV3DCmd::MenuPopup:                        return TEXT("Menu Popup");
        case EV3DCmd::MenuButtonMappingEditor:          return TEXT("Menu Button Mapping Editor");
        case EV3DCmd::MenuAdvancedSettingsEditor:       return TEXT("Menu Advanced Settings Editor");
        case EV3DCmd::MotionMacroZoom:                  return TEXT("Motion Macro Zoom");
        case EV3DCmd::MotionMacroZoomOutCursorToCenter: return TEXT("Motion Macro Zoom Out Cursor To Center");
        case EV3DCmd::MotionMacroZoomInCursorToCenter:  return TEXT("Motion Macro Zoom In Cursor To Center");
        case EV3DCmd::MotionMacroZoomOutCenterToCenter: return TEXT("Motion Macro Zoom Out Center To Center");
        case EV3DCmd::MotionMacroZoomInCenterToCenter:  return TEXT("Motion Macro Zoom In Center To Center");
        case EV3DCmd::MotionMacroZoomOutCursorToCursor: return TEXT("Motion Macro Zoom Out Cursor To Cursor");
        case EV3DCmd::MotionMacroZoomInCursorToCursor:  return TEXT("Motion Macro Zoom In Cursor To Cursor");
        case EV3DCmd::ViewQzIn:                         return TEXT("View Qz In");
        case EV3DCmd::ViewQzOut:                        return TEXT("View Qz Out");
        case EV3DCmd::KeyEnter:                         return TEXT("Key Enter");
        case EV3DCmd::KeyDelete:                        return TEXT("Key Delete");
        case EV3DCmd::KeyF13:                           return TEXT("Key F13");
        case EV3DCmd::KeyF14:                           return TEXT("Key F14");
        case EV3DCmd::KeyF15:                           return TEXT("Key F15");
        case EV3DCmd::KeyF16:                           return TEXT("Key F16");
        case EV3DCmd::KeyF17:                           return TEXT("Key F17");
        case EV3DCmd::KeyF18:                           return TEXT("Key F18");
        case EV3DCmd::KeyF19:                           return TEXT("Key F19");
        case EV3DCmd::KeyF20:                           return TEXT("Key F20");
        case EV3DCmd::KeyF21:                           return TEXT("Key F21");
        case EV3DCmd::KeyF22:                           return TEXT("Key F22");
        case EV3DCmd::KeyF23:                           return TEXT("Key F23");
        case EV3DCmd::KeyF24:                           return TEXT("Key F24");
        case EV3DCmd::KeyF25:                           return TEXT("Key F25");
        case EV3DCmd::KeyF26:                           return TEXT("Key F26");
        case EV3DCmd::KeyF27:                           return TEXT("Key F27");
        case EV3DCmd::KeyF28:                           return TEXT("Key F28");
        case EV3DCmd::KeyF29:                           return TEXT("Key F29");
        case EV3DCmd::KeyF30:                           return TEXT("Key F30");
        case EV3DCmd::KeyF31:                           return TEXT("Key F31");
        case EV3DCmd::KeyF32:                           return TEXT("Key F32");
        case EV3DCmd::KeyF33:                           return TEXT("Key F33");
        case EV3DCmd::KeyF34:                           return TEXT("Key F34");
        case EV3DCmd::KeyF35:                           return TEXT("Key F35");
        case EV3DCmd::KeyF36:                           return TEXT("Key F36");
        case EV3DCmd::View1:                            return TEXT("View 1");
        case EV3DCmd::View2:                            return TEXT("View 2");
        case EV3DCmd::View3:                            return TEXT("View 3");
        case EV3DCmd::View4:                            return TEXT("View 4");
        case EV3DCmd::View5:                            return TEXT("View 5");
        case EV3DCmd::View6:                            return TEXT("View 6");
        case EV3DCmd::View7:                            return TEXT("View 7");
        case EV3DCmd::View8:                            return TEXT("View 8");
        case EV3DCmd::View9:                            return TEXT("View 9");
        case EV3DCmd::View10:                           return TEXT("View 10");
        case EV3DCmd::View11:                           return TEXT("View 11");
        case EV3DCmd::View12:                           return TEXT("View 12");
        case EV3DCmd::View13:                           return TEXT("View 13");
        case EV3DCmd::View14:                           return TEXT("View 14");
        case EV3DCmd::View15:                           return TEXT("View 15");
        case EV3DCmd::View16:                           return TEXT("View 16");
        case EV3DCmd::View17:                           return TEXT("View 17");
        case EV3DCmd::View18:                           return TEXT("View 18");
        case EV3DCmd::View19:                           return TEXT("View 19");
        case EV3DCmd::View20:                           return TEXT("View 20");
        case EV3DCmd::View21:                           return TEXT("View 21");
        case EV3DCmd::View22:                           return TEXT("View 22");
        case EV3DCmd::View23:                           return TEXT("View 23");
        case EV3DCmd::View24:                           return TEXT("View 24");
        case EV3DCmd::View25:                           return TEXT("View 25");
        case EV3DCmd::View26:                           return TEXT("View 26");
        case EV3DCmd::View27:                           return TEXT("View 27");
        case EV3DCmd::View28:                           return TEXT("View 28");
        case EV3DCmd::View29:                           return TEXT("View 29");
        case EV3DCmd::View30:                           return TEXT("View 30");
        case EV3DCmd::View31:                           return TEXT("View 31");
        case EV3DCmd::View32:                           return TEXT("View 32");
        case EV3DCmd::View33:                           return TEXT("View 33");
        case EV3DCmd::View34:                           return TEXT("View 34");
        case EV3DCmd::View35:                           return TEXT("View 35");
        case EV3DCmd::View36:                           return TEXT("View 36");
        case EV3DCmd::SaveView1:                        return TEXT("Save View 1");
        case EV3DCmd::SaveView2:                        return TEXT("Save View 2");
        case EV3DCmd::SaveView3:                        return TEXT("Save View 3");
        case EV3DCmd::SaveView4:                        return TEXT("Save View 4");
        case EV3DCmd::SaveView5:                        return TEXT("Save View 5");
        case EV3DCmd::SaveView6:                        return TEXT("Save View 6");
        case EV3DCmd::SaveView7:                        return TEXT("Save View 7");
        case EV3DCmd::SaveView8:                        return TEXT("Save View 8");
        case EV3DCmd::SaveView9:                        return TEXT("Save View 9");
        case EV3DCmd::SaveView10:                       return TEXT("Save View 10");
        case EV3DCmd::SaveView11:                       return TEXT("Save View 11");
        case EV3DCmd::SaveView12:                       return TEXT("Save View 12");
        case EV3DCmd::SaveView13:                       return TEXT("Save View 13");
        case EV3DCmd::SaveView14:                       return TEXT("Save View 14");
        case EV3DCmd::SaveView15:                       return TEXT("Save View 15");
        case EV3DCmd::SaveView16:                       return TEXT("Save View 16");
        case EV3DCmd::SaveView17:                       return TEXT("Save View 17");
        case EV3DCmd::SaveView18:                       return TEXT("Save View 18");
        case EV3DCmd::SaveView19:                       return TEXT("Save View 19");
        case EV3DCmd::SaveView20:                       return TEXT("Save View 20");
        case EV3DCmd::SaveView21:                       return TEXT("Save View 21");
        case EV3DCmd::SaveView22:                       return TEXT("Save View 22");
        case EV3DCmd::SaveView23:                       return TEXT("Save View 23");
        case EV3DCmd::SaveView24:                       return TEXT("Save View 24");
        case EV3DCmd::SaveView25:                       return TEXT("Save View 25");
        case EV3DCmd::SaveView26:                       return TEXT("Save View 26");
        case EV3DCmd::SaveView27:                       return TEXT("Save View 27");
        case EV3DCmd::SaveView28:                       return TEXT("Save View 28");
        case EV3DCmd::SaveView29:                       return TEXT("Save View 29");
        case EV3DCmd::SaveView30:                       return TEXT("Save View 30");
        case EV3DCmd::SaveView31:                       return TEXT("Save View 31");
        case EV3DCmd::SaveView32:                       return TEXT("Save View 32");
        case EV3DCmd::SaveView33:                       return TEXT("Save View 33");
        case EV3DCmd::SaveView34:                       return TEXT("Save View 34");
        case EV3DCmd::SaveView35:                       return TEXT("Save View 35");
        case EV3DCmd::SaveView36:                       return TEXT("Save View 36");
        case EV3DCmd::KeyTab:                           return TEXT("Key Tab");
        case EV3DCmd::KeySpace:                         return TEXT("Key Space");
        case EV3DCmd::Menu1:                            return TEXT("Menu 1");
        case EV3DCmd::Menu2:                            return TEXT("Menu 2");
        case EV3DCmd::Menu3:                            return TEXT("Menu 3");
        case EV3DCmd::Menu4:                            return TEXT("Menu 4");
        case EV3DCmd::Menu5:                            return TEXT("Menu 5");
        case EV3DCmd::Menu6:                            return TEXT("Menu 6");
        case EV3DCmd::Menu7:                            return TEXT("Menu 7");
        case EV3DCmd::Menu8:                            return TEXT("Menu 8");
        case EV3DCmd::Menu9:                            return TEXT("Menu 9");
        case EV3DCmd::Menu10:                           return TEXT("Menu 10");
        case EV3DCmd::Menu11:                           return TEXT("Menu 11");
        case EV3DCmd::Menu12:                           return TEXT("Menu 12");
        case EV3DCmd::Menu13:                           return TEXT("Menu 13");
        case EV3DCmd::Menu14:                           return TEXT("Menu 14");
        case EV3DCmd::Menu15:                           return TEXT("Menu 15");
        case EV3DCmd::Menu16:                           return TEXT("Menu 16");
        case EV3DCmd::Noop:                             return TEXT("Noop");
        default: return TEXT("Invalid");
    }
}
