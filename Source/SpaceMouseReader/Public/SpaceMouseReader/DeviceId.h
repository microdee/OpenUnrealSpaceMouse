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
#include "Mcro/Common.h"

namespace SpaceMouse::Reader
{
	using namespace Mcro::Common;

	class SPACEMOUSEREADER_API IDeviceIdBase : public IHaveType
	{
	public:
		// TODO: make TInherit support abstract classes
		virtual FString ToString() const;
		virtual uint32 GetHash() const;

		friend FORCEINLINE uint32 GetTypeHash(IDeviceIdBase const& self)
		{
			return self.GetHash();
		}
	};

	class SPACEMOUSEREADER_API ISpecificDeviceId : public TInherit<IDeviceIdBase> {};
	class SPACEMOUSEREADER_API IDeviceModelName : public TInherit<IDeviceIdBase> {};
	class SPACEMOUSEREADER_API FDeviceModelName : public TInherit<IDeviceModelName>
	{
	public:
		FDeviceModelName(FStringView const& name);
		FString Name;
		virtual FString ToString() const override;
		virtual uint32 GetHash() const override;
	};

	class SPACEMOUSEREADER_API FDeviceId : public IComposable
	{
	public:
		FString ToString() const;
		friend uint32 GetTypeHash(FDeviceId const& self);
	};
}
