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

#include "Runtime/Launch/Resources/Version.h"

#define MAKE_UE_VERSION(Major, Minor) (((Major) * 1000) + ((Minor) * 10))
#define MAKE_UE_VERSION_P(Major, Minor, Patch) (MAKE_UE_VERSION(Major, Minor) + (Patch))
#define UE_VERSION MAKE_UE_VERSION_P(ENGINE_MAJOR_VERSION, ENGINE_MINOR_VERSION, ENGINE_PATCH_VERSION)
