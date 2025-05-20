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
#include "SpaceMouseReader/Manager.h"

namespace SpaceMouse::Runtime
{
	using namespace SpaceMouse::Reader;
	
	class SPACEMOUSERUNTIME_API FRuntimeManager : public IManager
	{
	public:
		static FRuntimeManager& Get();
		virtual FSmUserSettings GetUserSettings() override;
	};
}