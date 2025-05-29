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

#include "SpaceMouseReader/Buttons.h"
#include "Mcro/TextMacros.h"

namespace SpaceMouse::Reader::Buttons
{
	using namespace Mcro::Common;

	FButtonQueue& FButtonQueue::operator |= (FButtonQueue const& other)
	{
		namespace rv = ranges::views;
		FButtonQueue selfCopy = *this;
		
		All = 0;
		int i = 0;
		for (uint16 button : rv::set_union(selfCopy, other))
		{
			if (i > 3) break;
			Array[i] = button;
			++i;
		}
		return *this;
	}

	FButtonQueue operator | (FButtonQueue const& l, FButtonQueue const& r)
	{
		FButtonQueue output(l);
		output |= r;
		return output;
	}

	TArrayView<uint16> FButtonQueue::ArrayView() const
	{
		return MakeArrayView(AsMutablePtr(&Array[0]), size());
	}
	
	const uint16* FButtonQueue::begin() const
	{
		return &Array[0];
	}

	const uint16* FButtonQueue::end() const
	{
		for (int i = 0; i < 4; ++i)
		{
			if (Array[i] == 0) return &Array[i];
		}
		return &Array[3] + 1;
	}

	size_t FButtonQueue::size() const
	{
		for (int i = 0; i < 4; ++i)
		{
			if (Array[i] == 0) return i;
		}
		return 4;
	}

	ranges::any_view<ECmd> AsCmd(FButtonQueue const& buttons)
	{
		return buttons
			| ranges::views::transform([](uint16 button)
			{
				return AsCmd(button - 1);
			});
	}

	FStringView GetFriendlyName(ECmd id)
	{
		switch (id)
		{
			case ECmd::MenuOptions:                      return TEXTVIEW_"Menu Options";
			case ECmd::ViewFit:                          return TEXTVIEW_"View Fit";
			case ECmd::ViewTop:                          return TEXTVIEW_"View Top";
			case ECmd::ViewLeft:                         return TEXTVIEW_"View Left";
			case ECmd::ViewRight:                        return TEXTVIEW_"View Right";
			case ECmd::ViewFront:                        return TEXTVIEW_"View Front";
			case ECmd::ViewBottom:                       return TEXTVIEW_"View Bottom";
			case ECmd::ViewBack:                         return TEXTVIEW_"View Back";
			case ECmd::ViewRollCW:                       return TEXTVIEW_"View Roll Cw";
			case ECmd::ViewRollCCW:                      return TEXTVIEW_"View Roll Ccw";
			case ECmd::ViewIso1:                         return TEXTVIEW_"View Iso 1";
			case ECmd::ViewIso2:                         return TEXTVIEW_"View Iso 2";
			case ECmd::KeyF1:                            return TEXTVIEW_"Key F1";
			case ECmd::KeyF2:                            return TEXTVIEW_"Key F2";
			case ECmd::KeyF3:                            return TEXTVIEW_"Key F3";
			case ECmd::KeyF4:                            return TEXTVIEW_"Key F4";
			case ECmd::KeyF5:                            return TEXTVIEW_"Key F5";
			case ECmd::KeyF6:                            return TEXTVIEW_"Key F6";
			case ECmd::KeyF7:                            return TEXTVIEW_"Key F7";
			case ECmd::KeyF8:                            return TEXTVIEW_"Key F8";
			case ECmd::KeyF9:                            return TEXTVIEW_"Key F9";
			case ECmd::KeyF10:                           return TEXTVIEW_"Key F10";
			case ECmd::KeyF11:                           return TEXTVIEW_"Key F11";
			case ECmd::KeyF12:                           return TEXTVIEW_"Key F12";
			case ECmd::KeyEsc:                           return TEXTVIEW_"Key Esc";
			case ECmd::KeyAlt:                           return TEXTVIEW_"Key Alt";
			case ECmd::KeyShift:                         return TEXTVIEW_"Key Shift";
			case ECmd::KeyCtrl:                          return TEXTVIEW_"Key Ctrl";
			case ECmd::FilterRotate:                     return TEXTVIEW_"Filter Rotate";
			case ECmd::FilterPanzoom:                    return TEXTVIEW_"Filter Panzoom";
			case ECmd::FilterDominant:                   return TEXTVIEW_"Filter Dominant";
			case ECmd::ScalePlus:                        return TEXTVIEW_"Scale Plus";
			case ECmd::ScaleMinus:                       return TEXTVIEW_"Scale Minus";
			case ECmd::ViewSpinCW:                       return TEXTVIEW_"View Spin CW";
			case ECmd::ViewSpinCCW:                      return TEXTVIEW_"View Spin CCW";
			case ECmd::ViewTiltCW:                       return TEXTVIEW_"View Tilt CW";
			case ECmd::ViewTiltCCW:                      return TEXTVIEW_"View Tilt CCW";
			case ECmd::MenuPopup:                        return TEXTVIEW_"Menu Popup";
			case ECmd::MenuButtonMappingEditor:          return TEXTVIEW_"Menu Button Mapping Editor";
			case ECmd::MenuAdvancedSettingsEditor:       return TEXTVIEW_"Menu Advanced Settings Editor";
			case ECmd::MotionMacroZoom:                  return TEXTVIEW_"Motion Macro Zoom";
			case ECmd::MotionMacroZoomOutCursorToCenter: return TEXTVIEW_"Motion Macro Zoom Out Cursor To Center";
			case ECmd::MotionMacroZoomInCursorToCenter:  return TEXTVIEW_"Motion Macro Zoom In Cursor To Center";
			case ECmd::MotionMacroZoomOutCenterToCenter: return TEXTVIEW_"Motion Macro Zoom Out Center To Center";
			case ECmd::MotionMacroZoomInCenterToCenter:  return TEXTVIEW_"Motion Macro Zoom In Center To Center";
			case ECmd::MotionMacroZoomOutCursorToCursor: return TEXTVIEW_"Motion Macro Zoom Out Cursor To Cursor";
			case ECmd::MotionMacroZoomInCursorToCursor:  return TEXTVIEW_"Motion Macro Zoom In Cursor To Cursor";
			case ECmd::ViewQzIn:                         return TEXTVIEW_"View Qz In";
			case ECmd::ViewQzOut:                        return TEXTVIEW_"View Qz Out";
			case ECmd::KeyEnter:                         return TEXTVIEW_"Key Enter";
			case ECmd::KeyDelete:                        return TEXTVIEW_"Key Delete";
			case ECmd::KeyF13:                           return TEXTVIEW_"Key F13";
			case ECmd::KeyF14:                           return TEXTVIEW_"Key F14";
			case ECmd::KeyF15:                           return TEXTVIEW_"Key F15";
			case ECmd::KeyF16:                           return TEXTVIEW_"Key F16";
			case ECmd::KeyF17:                           return TEXTVIEW_"Key F17";
			case ECmd::KeyF18:                           return TEXTVIEW_"Key F18";
			case ECmd::KeyF19:                           return TEXTVIEW_"Key F19";
			case ECmd::KeyF20:                           return TEXTVIEW_"Key F20";
			case ECmd::KeyF21:                           return TEXTVIEW_"Key F21";
			case ECmd::KeyF22:                           return TEXTVIEW_"Key F22";
			case ECmd::KeyF23:                           return TEXTVIEW_"Key F23";
			case ECmd::KeyF24:                           return TEXTVIEW_"Key F24";
			case ECmd::KeyF25:                           return TEXTVIEW_"Key F25";
			case ECmd::KeyF26:                           return TEXTVIEW_"Key F26";
			case ECmd::KeyF27:                           return TEXTVIEW_"Key F27";
			case ECmd::KeyF28:                           return TEXTVIEW_"Key F28";
			case ECmd::KeyF29:                           return TEXTVIEW_"Key F29";
			case ECmd::KeyF30:                           return TEXTVIEW_"Key F30";
			case ECmd::KeyF31:                           return TEXTVIEW_"Key F31";
			case ECmd::KeyF32:                           return TEXTVIEW_"Key F32";
			case ECmd::KeyF33:                           return TEXTVIEW_"Key F33";
			case ECmd::KeyF34:                           return TEXTVIEW_"Key F34";
			case ECmd::KeyF35:                           return TEXTVIEW_"Key F35";
			case ECmd::KeyF36:                           return TEXTVIEW_"Key F36";
			case ECmd::View1:                            return TEXTVIEW_"View 1";
			case ECmd::View2:                            return TEXTVIEW_"View 2";
			case ECmd::View3:                            return TEXTVIEW_"View 3";
			case ECmd::View4:                            return TEXTVIEW_"View 4";
			case ECmd::View5:                            return TEXTVIEW_"View 5";
			case ECmd::View6:                            return TEXTVIEW_"View 6";
			case ECmd::View7:                            return TEXTVIEW_"View 7";
			case ECmd::View8:                            return TEXTVIEW_"View 8";
			case ECmd::View9:                            return TEXTVIEW_"View 9";
			case ECmd::View10:                           return TEXTVIEW_"View 10";
			case ECmd::View11:                           return TEXTVIEW_"View 11";
			case ECmd::View12:                           return TEXTVIEW_"View 12";
			case ECmd::View13:                           return TEXTVIEW_"View 13";
			case ECmd::View14:                           return TEXTVIEW_"View 14";
			case ECmd::View15:                           return TEXTVIEW_"View 15";
			case ECmd::View16:                           return TEXTVIEW_"View 16";
			case ECmd::View17:                           return TEXTVIEW_"View 17";
			case ECmd::View18:                           return TEXTVIEW_"View 18";
			case ECmd::View19:                           return TEXTVIEW_"View 19";
			case ECmd::View20:                           return TEXTVIEW_"View 20";
			case ECmd::View21:                           return TEXTVIEW_"View 21";
			case ECmd::View22:                           return TEXTVIEW_"View 22";
			case ECmd::View23:                           return TEXTVIEW_"View 23";
			case ECmd::View24:                           return TEXTVIEW_"View 24";
			case ECmd::View25:                           return TEXTVIEW_"View 25";
			case ECmd::View26:                           return TEXTVIEW_"View 26";
			case ECmd::View27:                           return TEXTVIEW_"View 27";
			case ECmd::View28:                           return TEXTVIEW_"View 28";
			case ECmd::View29:                           return TEXTVIEW_"View 29";
			case ECmd::View30:                           return TEXTVIEW_"View 30";
			case ECmd::View31:                           return TEXTVIEW_"View 31";
			case ECmd::View32:                           return TEXTVIEW_"View 32";
			case ECmd::View33:                           return TEXTVIEW_"View 33";
			case ECmd::View34:                           return TEXTVIEW_"View 34";
			case ECmd::View35:                           return TEXTVIEW_"View 35";
			case ECmd::View36:                           return TEXTVIEW_"View 36";
			case ECmd::SaveView1:                        return TEXTVIEW_"Save View 1";
			case ECmd::SaveView2:                        return TEXTVIEW_"Save View 2";
			case ECmd::SaveView3:                        return TEXTVIEW_"Save View 3";
			case ECmd::SaveView4:                        return TEXTVIEW_"Save View 4";
			case ECmd::SaveView5:                        return TEXTVIEW_"Save View 5";
			case ECmd::SaveView6:                        return TEXTVIEW_"Save View 6";
			case ECmd::SaveView7:                        return TEXTVIEW_"Save View 7";
			case ECmd::SaveView8:                        return TEXTVIEW_"Save View 8";
			case ECmd::SaveView9:                        return TEXTVIEW_"Save View 9";
			case ECmd::SaveView10:                       return TEXTVIEW_"Save View 10";
			case ECmd::SaveView11:                       return TEXTVIEW_"Save View 11";
			case ECmd::SaveView12:                       return TEXTVIEW_"Save View 12";
			case ECmd::SaveView13:                       return TEXTVIEW_"Save View 13";
			case ECmd::SaveView14:                       return TEXTVIEW_"Save View 14";
			case ECmd::SaveView15:                       return TEXTVIEW_"Save View 15";
			case ECmd::SaveView16:                       return TEXTVIEW_"Save View 16";
			case ECmd::SaveView17:                       return TEXTVIEW_"Save View 17";
			case ECmd::SaveView18:                       return TEXTVIEW_"Save View 18";
			case ECmd::SaveView19:                       return TEXTVIEW_"Save View 19";
			case ECmd::SaveView20:                       return TEXTVIEW_"Save View 20";
			case ECmd::SaveView21:                       return TEXTVIEW_"Save View 21";
			case ECmd::SaveView22:                       return TEXTVIEW_"Save View 22";
			case ECmd::SaveView23:                       return TEXTVIEW_"Save View 23";
			case ECmd::SaveView24:                       return TEXTVIEW_"Save View 24";
			case ECmd::SaveView25:                       return TEXTVIEW_"Save View 25";
			case ECmd::SaveView26:                       return TEXTVIEW_"Save View 26";
			case ECmd::SaveView27:                       return TEXTVIEW_"Save View 27";
			case ECmd::SaveView28:                       return TEXTVIEW_"Save View 28";
			case ECmd::SaveView29:                       return TEXTVIEW_"Save View 29";
			case ECmd::SaveView30:                       return TEXTVIEW_"Save View 30";
			case ECmd::SaveView31:                       return TEXTVIEW_"Save View 31";
			case ECmd::SaveView32:                       return TEXTVIEW_"Save View 32";
			case ECmd::SaveView33:                       return TEXTVIEW_"Save View 33";
			case ECmd::SaveView34:                       return TEXTVIEW_"Save View 34";
			case ECmd::SaveView35:                       return TEXTVIEW_"Save View 35";
			case ECmd::SaveView36:                       return TEXTVIEW_"Save View 36";
			case ECmd::KeyTab:                           return TEXTVIEW_"Key Tab";
			case ECmd::KeySpace:                         return TEXTVIEW_"Key Space";
			case ECmd::Menu1:                            return TEXTVIEW_"Menu 1";
			case ECmd::Menu2:                            return TEXTVIEW_"Menu 2";
			case ECmd::Menu3:                            return TEXTVIEW_"Menu 3";
			case ECmd::Menu4:                            return TEXTVIEW_"Menu 4";
			case ECmd::Menu5:                            return TEXTVIEW_"Menu 5";
			case ECmd::Menu6:                            return TEXTVIEW_"Menu 6";
			case ECmd::Menu7:                            return TEXTVIEW_"Menu 7";
			case ECmd::Menu8:                            return TEXTVIEW_"Menu 8";
			case ECmd::Menu9:                            return TEXTVIEW_"Menu 9";
			case ECmd::Menu10:                           return TEXTVIEW_"Menu 10";
			case ECmd::Menu11:                           return TEXTVIEW_"Menu 11";
			case ECmd::Menu12:                           return TEXTVIEW_"Menu 12";
			case ECmd::Menu13:                           return TEXTVIEW_"Menu 13";
			case ECmd::Menu14:                           return TEXTVIEW_"Menu 14";
			case ECmd::Menu15:                           return TEXTVIEW_"Menu 15";
			case ECmd::Menu16:                           return TEXTVIEW_"Menu 16";
			case ECmd::Noop:                             return TEXTVIEW_"Noop";
			default: return TEXTVIEW_"Invalid";
		}
	}

	void ButtonBitsToQueue(uint32 buttonBits, FButtonQueue& output)
	{
		output.All = 0;
		int32 current = 0;
		for (int i = 0; i < 32; ++i)
		{
			if (buttonBits & 1 << i)
			{
				output.Array[current++] = i + 1;
				if (current > 3) break;
			}
		}
	}

	void MapButtonsFromArray(FButtonQueue& target, TArray<ECmd> const& array)
	{
		for (int i = 0; i < 4; ++i)
		{
			if (target.Array[i] > 0)
				target.Array[i] = AsUint16(array[target.Array[i] - 1]) + 1;
		}
	}

	void Map3DConnexionModern(FButtonQueue& target)
	{
		for (int i = 0; i < 4; ++i)
			target.Array[i] = AsUint16(Map3DConnexionModern(target.Array[i] - 1)) + 1;
	}

	ECmd Map3DConnexionModern(uint16 rawId)
	{
		// Source of info: https://github.com/blender/blender/blob/594f47ecd2d5367ca936cf6fc6ec8168c2b360d0/intern/ghost/intern/GHOST_NDOFManager.cpp#L91
		switch (rawId)
		{
			case   0: return ECmd::MenuOptions;
			case   1: return ECmd::ViewFit;
			case   2: return ECmd::ViewTop;
			case   3: return ECmd::ViewLeft;
			case   4: return ECmd::ViewRight;
			case   5: return ECmd::ViewFront;
			case   6: return ECmd::ViewBottom;
			case   7: return ECmd::ViewBack;
			case   8: return ECmd::ViewRollCW;
			case   9: return ECmd::ViewRollCCW;
			case  10: return ECmd::ViewIso1;
			case  11: return ECmd::ViewIso2;
			case  12: return ECmd::KeyF1;
			case  13: return ECmd::KeyF2;
			case  14: return ECmd::KeyF3;
			case  15: return ECmd::KeyF4;
			case  16: return ECmd::KeyF5;
			case  17: return ECmd::KeyF6;
			case  18: return ECmd::KeyF7;
			case  19: return ECmd::KeyF8;
			case  20: return ECmd::KeyF9;
			case  21: return ECmd::KeyF10;
			case  22: return ECmd::KeyEsc;
			case  23: return ECmd::KeyAlt;
			case  24: return ECmd::KeyShift;
			case  25: return ECmd::KeyCtrl;
			case  26: return ECmd::FilterRotate;
			case  27: return ECmd::FilterPanzoom;
			case  28: return ECmd::FilterDominant;
			case  29: return ECmd::ScalePlus;
			case  30: return ECmd::ScaleMinus;

			// Only available on button queue input (report 28)
			case  35: return ECmd::KeyEnter;
			case  36: return ECmd::KeyDelete;
			case  76: return ECmd::KeyF11;
			case  77: return ECmd::KeyF12;
			case 102: return ECmd::View1;
			case 103: return ECmd::View2;
			case 104: return ECmd::View3;
			case 174: return ECmd::KeyTab;
			case 175: return ECmd::KeySpace;
			default:  return ECmd::Noop;
		}
	}
}
