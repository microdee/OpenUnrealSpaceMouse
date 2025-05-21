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

#include "SpaceMouseReader/DeviceId.h"

namespace SpaceMouse::Reader
{
	uint32 GetTypeHash(FDeviceId const& self)
	{
		return ranges::accumulate(
			self.GetComponents<IDeviceIdBase>(),
			0U,
			[](uint32 const& t, uint32 const& u) { return t ^ u; },
			[](IDeviceIdBase* i) { return GetTypeHash(&i); }
		);
	}

	FString IDeviceIdBase::ToString() const { FORCE_CRASH(); return {}; }

	uint32 IDeviceIdBase::GetHash() const { FORCE_CRASH(); return 0; }

	FDeviceModelName::FDeviceModelName(FStringView const& name)
		: Name(name)
	{}

	FString FDeviceModelName::ToString() const
	{
		return Name;
	}

	uint32 FDeviceModelName::GetHash() const
	{
		return GetTypeHash(Name);
	}

	FString FDeviceId::ToString() const
	{
		namespace rv = ranges::views; 
		return GetComponents<IDeviceIdBase>()
			| rv::transform([](const IDeviceIdBase* i)
			{
				return TEXT_"{0}: ({1})" _FMT(i->GetType(), i);
			})
			| RenderAsString();
	}
}
