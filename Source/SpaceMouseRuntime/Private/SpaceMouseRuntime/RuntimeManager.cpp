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
#include "SpaceMouseRuntime.h"

namespace SpaceMouse::Runtime
{
	TModuleBoundObject<FSpaceMouseRuntimeModule, FRuntimeManager> GRuntimeManager {};
	
	FRuntimeManager& FRuntimeManager::Get()
	{
		return GRuntimeManager.GetChecked();
	}

	FSmUserSettings FRuntimeManager::GetUserSettings()
	{
		// TODO: support global runtime settings
		static FSmUserSettings settings {};
		return settings;
	}
}
