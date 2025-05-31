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
#include "Buttons.h"
#include "Containers/StaticBitArray.h"
#include "Mcro/CommonCore.h"

namespace SpaceMouse::Reader::Buttons
{
	using namespace Mcro::Concepts;
	
	enum class ECmd : uint8;

	union SPACEMOUSEREADER_API FButtonQueue
	{
		uint16 Array [4];
		uint64 All;

		FButtonQueue& operator |= (FButtonQueue const& other);
		friend FButtonQueue operator | (FButtonQueue const& l, FButtonQueue const& r);

		TArrayView<uint16> ArrayView() const;

		const uint16* begin() const;
		const uint16* end() const;
		size_t size() const;
	};

	constexpr FORCEINLINE int32 AsInt(ECmd id)
	{
		return static_cast<int32>(id);
	}
	
	constexpr FORCEINLINE uint8 AsUint8(ECmd id)
	{
		return static_cast<uint8>(id);
	}
	
	constexpr FORCEINLINE uint8 AsUint16(ECmd id)
	{
		return static_cast<uint16>(id);
	}

	template <CIntegral T>
	constexpr ECmd AsCmd(T id)
	{
		return static_cast<ECmd>(id);
	}

	ranges::any_view<ECmd> AsCmd(FButtonQueue const& buttons);

	SPACEMOUSEREADER_API FStringView GetFriendlyName(ECmd id);

	SPACEMOUSEREADER_API void ButtonBitsToQueue(uint32 buttonBits, FButtonQueue& output);
	SPACEMOUSEREADER_API void MapButtonsFromArray(FButtonQueue& target, TArray<ECmd> const& array);
	SPACEMOUSEREADER_API void Map3DConnexionModern(FButtonQueue& target);
	SPACEMOUSEREADER_API ECmd Map3DConnexionModern(uint16 rawId);

	enum class ECmd : uint8
	{
		MenuOptions                      = 0,
		ViewFit                          = 1,
		ViewTop                          = 2,
		ViewLeft                         = 3,
		ViewRight                        = 4,
		ViewFront                        = 5,
		ViewBottom                       = 6,
		ViewBack                         = 7,
		ViewRollCW                       = 8,
		ViewRollCCW                      = 9,
		ViewIso1                         = 10,
		ViewIso2                         = 11,
		KeyF1                            = 12,
		KeyF2                            = 13,
		KeyF3                            = 14,
		KeyF4                            = 15,
		KeyF5                            = 16,
		KeyF6                            = 17,
		KeyF7                            = 18,
		KeyF8                            = 19,
		KeyF9                            = 20,
		KeyF10                           = 21,
		KeyF11                           = 22,
		KeyF12                           = 23,
		KeyEsc                           = 24,
		KeyAlt                           = 25,
		KeyShift                         = 26,
		KeyCtrl                          = 27,
		FilterRotate                     = 28,
		FilterPanzoom                    = 29,
		FilterDominant                   = 30,
		ScalePlus                        = 31,
		ScaleMinus                       = 32,
		ViewSpinCW                       = 33,
		ViewSpinCCW                      = 34,
		ViewTiltCW                       = 35,
		ViewTiltCCW                      = 36,
		MenuPopup                        = 37,
		MenuButtonMappingEditor          = 38,
		MenuAdvancedSettingsEditor       = 39,
		MotionMacroZoom                  = 40,
		MotionMacroZoomOutCursorToCenter = 41,
		MotionMacroZoomInCursorToCenter  = 42,
		MotionMacroZoomOutCenterToCenter = 43,
		MotionMacroZoomInCenterToCenter  = 44,
		MotionMacroZoomOutCursorToCursor = 45,
		MotionMacroZoomInCursorToCursor  = 46,
		ViewQzIn                         = 47,
		ViewQzOut                        = 48,
		KeyEnter                         = 49,
		KeyDelete                        = 50,
		KeyF13                           = 51,
		KeyF14                           = 52,
		KeyF15                           = 53,
		KeyF16                           = 54,
		KeyF17                           = 55,
		KeyF18                           = 56,
		KeyF19                           = 57,
		KeyF20                           = 58,
		KeyF21                           = 59,
		KeyF22                           = 60,
		KeyF23                           = 61,
		KeyF24                           = 62,
		KeyF25                           = 63,
		KeyF26                           = 64,
		KeyF27                           = 65,
		KeyF28                           = 66,
		KeyF29                           = 67,
		KeyF30                           = 68,
		KeyF31                           = 69,
		KeyF32                           = 70,
		KeyF33                           = 71,
		KeyF34                           = 72,
		KeyF35                           = 73,
		KeyF36                           = 74,
		View1                            = 75,
		View2                            = 76,
		View3                            = 77,
		View4                            = 78,
		View5                            = 79,
		View6                            = 80,
		View7                            = 81,
		View8                            = 82,
		View9                            = 83,
		View10                           = 84,
		View11                           = 85,
		View12                           = 86,
		View13                           = 87,
		View14                           = 88,
		View15                           = 89,
		View16                           = 90,
		View17                           = 91,
		View18                           = 92,
		View19                           = 93,
		View20                           = 94,
		View21                           = 95,
		View22                           = 96,
		View23                           = 97,
		View24                           = 98,
		View25                           = 99,
		View26                           = 100,
		View27                           = 101,
		View28                           = 102,
		View29                           = 103,
		View30                           = 104,
		View31                           = 105,
		View32                           = 106,
		View33                           = 107,
		View34                           = 108,
		View35                           = 109,
		View36                           = 110,
		SaveView1                        = 111,
		SaveView2                        = 112,
		SaveView3                        = 113,
		SaveView4                        = 114,
		SaveView5                        = 115,
		SaveView6                        = 116,
		SaveView7                        = 117,
		SaveView8                        = 118,
		SaveView9                        = 119,
		SaveView10                       = 120,
		SaveView11                       = 121,
		SaveView12                       = 122,
		SaveView13                       = 123,
		SaveView14                       = 124,
		SaveView15                       = 125,
		SaveView16                       = 126,
		SaveView17                       = 127,
		SaveView18                       = 128,
		SaveView19                       = 129,
		SaveView20                       = 130,
		SaveView21                       = 131,
		SaveView22                       = 132,
		SaveView23                       = 133,
		SaveView24                       = 134,
		SaveView25                       = 135,
		SaveView26                       = 136,
		SaveView27                       = 137,
		SaveView28                       = 138,
		SaveView29                       = 139,
		SaveView30                       = 140,
		SaveView31                       = 141,
		SaveView32                       = 142,
		SaveView33                       = 143,
		SaveView34                       = 144,
		SaveView35                       = 145,
		SaveView36                       = 146,
		KeyTab                           = 147,
		KeySpace                         = 148,
		Menu1                            = 149,
		Menu2                            = 150,
		Menu3                            = 151,
		Menu4                            = 152,
		Menu5                            = 153,
		Menu6                            = 154,
		Menu7                            = 155,
		Menu8                            = 156,
		Menu9                            = 157,
		Menu10                           = 158,
		Menu11                           = 159,
		Menu12                           = 160,
		Menu13                           = 161,
		Menu14                           = 162,
		Menu15                           = 163,
		Menu16                           = 164,
		
		COUNT                            = 165,
		Noop                             = 0xFF,
	};
}

namespace magic_enum::customize
{
	using namespace SpaceMouse::Reader::Buttons;
	
	template <>
	struct enum_range<ECmd> {
		static constexpr int min = 0;
		static constexpr int max = static_cast<int>(ECmd::COUNT);
		// (max - min) must be less than UINT16_MAX.
	};
}
