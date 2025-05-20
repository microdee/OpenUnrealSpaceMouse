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

#include "SpaceMouseRuntime/RuntimeManager.h"

namespace SpaceMouse::Runtime
{
	FRuntimeManager GRuntimeManager {};
	
	FRuntimeManager& FRuntimeManager::Get()
	{
		return GRuntimeManager;
	}

	FSmUserSettings FRuntimeManager::GetUserSettings()
	{
		static FSmUserSettings settings {};
		return settings;
	}
}
