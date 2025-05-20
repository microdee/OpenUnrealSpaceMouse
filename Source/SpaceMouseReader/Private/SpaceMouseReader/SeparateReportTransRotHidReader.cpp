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




#include "SpaceMouseReader/SeparateReportTransRotHidReader.h"
#include "SpaceMouseReader/MovementState.h"

namespace SpaceMouse::Reader
{
    FSeparateReportTransRotHidReader::FSeparateReportTransRotHidReader(Hid::FHidDeviceInfo const& devInfo)
        : Device(devInfo)
    {
    }

    void FSeparateReportTransRotHidReader::ReadData(FTickArgs& output, float deltaSecs)
    {
        // TODO: debug
        // Output.Debug->AppendReport(Report, GetReportSize());
        int32 report = 0;
        for (int i = 0; i < GetReportCount(); i++)
        {
            const uint8 reportID = OutputBuffer[report];

            if (reportID == 1 || reportID == 2)
            {
                int16 xx = *reinterpret_cast<int16*>(&OutputBuffer[report + 1]);
                int16 yy = *reinterpret_cast<int16*>(&OutputBuffer[report + 3]);
                int16 zz = *reinterpret_cast<int16*>(&OutputBuffer[report + 5]);

                if (!CheckAxes(xx, yy, zz)) continue;

                float fx = static_cast<float>(xx) / GetAxisResolution();
                float fy = static_cast<float>(yy) / GetAxisResolution();
                float fz = static_cast<float>(zz) / GetAxisResolution();

                output.MovementState.FlagMove();
                if (reportID == 1) ApplyTranslation(output, fx, fy, fz, deltaSecs);
                if (reportID == 2) ApplyRotation(output, fx, fy, fz, deltaSecs);
            }
            else if (reportID == 3)
            {
                uint32 buttonBits = *reinterpret_cast<uint32*>(&OutputBuffer[report + 1]);
                Buttons::ButtonBitsToQueue(buttonBits, output.ProcessedData.ButtonQueue);
                Buttons::ButtonBitsToQueue(buttonBits, output.NormData.ButtonQueue);
            }
            report += GetReportSize();
        }
    }

    Hid::FScopedHidDevice const& FSeparateReportTransRotHidReader::GetDevice()
    {
        return Device;
    }
}
