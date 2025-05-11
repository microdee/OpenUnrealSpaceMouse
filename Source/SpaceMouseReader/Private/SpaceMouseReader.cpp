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



#include "SpaceMouseReader.h"

#include "hidapi.h"

DEFINE_LOG_CATEGORY(LogSmReader);

void FSpaceMouseReaderModule::StartupModule()
{
    hid_init();
}

void FSpaceMouseReaderModule::ShutdownModule()
{
    hid_exit();
}

IMPLEMENT_MODULE(FSpaceMouseReaderModule, SpaceMouseReader)
