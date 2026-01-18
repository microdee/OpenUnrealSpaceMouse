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

#include "CoreMinimal.h"
#include "HAL/PlatformFileManager.h"
#include "Misc/FileHelper.h"
#include "SpaceMouseRuntime.h"
#include "Mcro/Common.h"

#if PLATFORM_WINDOWS

DECLARE_LOG_CATEGORY_CLASS(LogSpaceMouseDisableTDxWareBindings, Log, Log);

namespace
{
	using namespace Mcro::Common;

	TObserveModule<FSpaceMouseRuntimeModule> GObserveModule
	{{
		.OnStartup = []
		{
			auto&& platformFile = FPlatformFileManager::Get().GetPlatformFile();
			auto tdxConfigDir = FPlatformProcess::UserHomeDir()
				/ FString(TEXT_"AppData/Roaming/3Dconnexion/3DxWare/Cfg")
			;
			if (!platformFile.DirectoryExists(*tdxConfigDir))
			{
				platformFile.CreateDirectoryTree(*tdxConfigDir);
			}
			if (auto thisPlugin = IPluginManager::Get().FindPlugin("OpenUnrealSpaceMouse"))
			{
				auto programName = FPlatformProcess::ExecutableName();

				auto disableKmjTemplatePath = thisPlugin->GetContentDir() / TEXT_"Disable-KMJ.xml";
				FMT_LOG(LogSpaceMouseDisableTDxWareBindings, Display, "Rendering 3DxWare settings from {0}", disableKmjTemplatePath);
				FMT_LOG(LogSpaceMouseDisableTDxWareBindings, Display, "    For {0}", programName);

				FString disableKmj;
				FFileHelper::LoadFileToString(disableKmj, *disableKmjTemplatePath);
				disableKmj.ReplaceInline(TEXT_"{{ AppName }}", programName);

				auto outputPath = tdxConfigDir / (TEXT_"{0}-KMJ.xml" _FMT(programName));
				FMT_LOG(LogSpaceMouseDisableTDxWareBindings, Display, "    To {0}", outputPath);

				FFileHelper::SaveStringToFile(disableKmj, *outputPath);
			}
		}
	}};
}

#endif
