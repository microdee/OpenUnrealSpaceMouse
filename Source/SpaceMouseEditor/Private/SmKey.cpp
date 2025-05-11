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



// ReSharper disable CppNonExplicitConvertingConstructor
// ReSharper disable CppNonExplicitConversionOperator
#include "SmKey.h"

FSmKey::FSmKey(const FKey& InKey)
    : Key(InKey)
{
}

FSmKey::FSmKey(const FName InName)
    : Key(InName)
{
}

FSmKey::FSmKey(const TCHAR* InName)
    : Key(InName)
{
}

FSmKey::FSmKey(const ANSICHAR* InName)
    : Key(InName)
{
}

bool FSmKey::SerializeFromMismatchedTag(FPropertyTag const& Tag, FStructuredArchive::FSlot Slot)
{
    return Key.SerializeFromMismatchedTag(Tag, Slot);
}

bool FSmKey::ExportTextItem(FString& ValueStr, FKey const& DefaultValue, UObject* Parent, int32 PortFlags, UObject* ExportRootScope) const
{
    return Key.ExportTextItem(ValueStr, DefaultValue, Parent, PortFlags, ExportRootScope);
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
