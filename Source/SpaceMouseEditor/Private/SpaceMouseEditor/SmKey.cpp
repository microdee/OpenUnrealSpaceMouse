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

#include "SmKey.h"

bool FSmKey::SerializeFromMismatchedTag(FPropertyTag const& Tag, FStructuredArchive::FSlot Slot)
{
    return Key.SerializeFromMismatchedTag(Tag, Slot);
}

bool FSmKey::ExportTextItem(FString& ValueStr, FSmKey const& DefaultValue, UObject* Parent, int32 PortFlags, UObject* ExportRootScope) const
{
    return Key.ExportTextItem(ValueStr, DefaultValue.Key, Parent, PortFlags, ExportRootScope);
}

bool FSmKey::ImportTextItem(const TCHAR*& Buffer, int32 PortFlags, UObject* Parent, FOutputDevice* ErrorText)
{
    return Key.ImportTextItem(Buffer, PortFlags, Parent, ErrorText);
}

void FSmKey::PostSerialize(const FArchive& Ar)
{
    Key.PostSerialize(Ar);
}

void FSmKey::PostScriptConstruct()
{
    Key.PostScriptConstruct();
}

FSmKey::operator FKey() const
{
    return Key;
}
