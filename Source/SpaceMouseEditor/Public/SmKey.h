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
#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "InputCore.h"
#include "SmKey.generated.h"

/**
 * A replacement struct for FKeys, only used for custom SpaceMouse key selector
 */
USTRUCT(BlueprintType)
struct SPACEMOUSEEDITOR_API FSmKey
{
    GENERATED_BODY()

    FSmKey() {}
    FSmKey(const FKey& InKey);
    FSmKey(const FName InName);
    FSmKey(const TCHAR* InName);
    FSmKey(const ANSICHAR* InName);
    
    //UPROPERTY(BlueprintReadWrite)
    
    FKey Key {};
    
    bool SerializeFromMismatchedTag(struct FPropertyTag const& Tag, FStructuredArchive::FSlot Slot);
    bool ExportTextItem(FString& ValueStr, FKey const& DefaultValue, UObject* Parent, int32 PortFlags, UObject* ExportRootScope) const;
    bool ImportTextItem(const TCHAR*& Buffer, int32 PortFlags, UObject* Parent, FOutputDevice* ErrorText);
    void PostSerialize(const FArchive& Ar);
    void PostScriptConstruct();

    operator FKey() const;

    friend bool operator==(const FSmKey& Lhs, const FSmKey& RHS) { return Lhs.Key == RHS.Key; }
    friend bool operator!=(const FSmKey& Lhs, const FSmKey& RHS) { return !(Lhs == RHS); }
    
    friend uint32 GetTypeHash(const FSmKey& InKey) { return GetTypeHash(InKey.Key); }
};

template<>
struct TStructOpsTypeTraits<FSmKey> : public TStructOpsTypeTraitsBase2<FSmKey>
{
    enum
    {
        WithStructuredSerializeFromMismatchedTag = true,
        WithExportTextItem = true,
        WithImportTextItem = true,
        WithPostSerialize = true,
        WithPostScriptConstruct = true,
        WithCopy = true,		// Necessary so that TSharedPtr<FKeyDetails> Data is copied around
    };
};
