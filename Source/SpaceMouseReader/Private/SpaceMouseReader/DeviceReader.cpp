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


#include "SpaceMouseReader/DeviceReader.h"

namespace SpaceMouse::Reader
{
	template<typename TResult>
	void ApplyMovement(float fx, float fy, float fz, float DeltaSecs, FSmMovementSettings const& Settings, TResult& Output, TResult& NormOutput)
	{
		FVector xmap = Settings.XAxisMap;
		FVector ymap = Settings.YAxisMap;
		FVector zmap = Settings.ZAxisMap;

		fx = IDeviceReader::GetCurvedFloat(Settings.Curve, fx);
		fy = IDeviceReader::GetCurvedFloat(Settings.Curve, fy);
		fz = IDeviceReader::GetCurvedFloat(Settings.Curve, fz);

		NormOutput = TResult(
			fx * xmap.X + fy * xmap.Y + fz * xmap.Z,
			fx * ymap.X + fy * ymap.Y + fz * ymap.Z,
			fx * zmap.X + fy * zmap.Y + fz * zmap.Z
		);
        
		Output = NormOutput * Settings.UnitsPerSec * DeltaSecs;
	}

	float IDeviceReader::GetCurvedFloat(FRichCurve const& curve, float ff)
	{
		if(!curve.IsEmpty() && FMath::Abs(ff) > SMALL_NUMBER)
			return curve.Eval(FMath::Abs(ff), FMath::Abs(ff)) * FMath::Sign(ff);
		return ff;
	}

	void IDeviceReader::Tick(FTickArgs&& output, float deltaSecs)
	{
		Tick(output, deltaSecs);
	}

	void IDeviceReader::Tick(FTickArgs& output, float deltaSecs)
	{
		output.MovementState.PreTick();
	}

	void IDeviceReader::TickMovementState(FTickArgs& output, float deltaSecs)
	{
		output.MovementState.Tick(output.Settings.MovementTimeTolerance, deltaSecs);
	}

	void IDeviceReader::ApplyTranslation(FTickArgs& output, float fx, float fy, float fz, float deltaSecs)
	{
		ApplyMovement(
			fx, fy, fz,
			deltaSecs,
			output.Settings.Translation,
			output.ProcessedData.Translation,
			output.NormData.Translation
		);
	}

	void IDeviceReader::ApplyRotation(FTickArgs& output, float fp, float fy, float fr, float deltaSecs)
	{
		ApplyMovement(
			fp, fy, fr,
			deltaSecs,
			output.Settings.Rotation,
			output.ProcessedData.Rotation,
			output.NormData.Rotation
		);
	}
}
