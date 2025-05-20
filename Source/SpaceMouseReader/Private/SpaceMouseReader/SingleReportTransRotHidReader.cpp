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




#include "SpaceMouseReader/SingleReportTransRotHidReader.h"
#include "SpaceMouseReader/MovementState.h"

namespace SpaceMouse::Reader
{
    FSingleReportTransRotHidReader::FSingleReportTransRotHidReader(Hid::FHidDeviceInfo const& devInfo, EButtonReportSource buttonSource)
        : Device(devInfo)
        , ButtonSource(buttonSource)
    {
    }

    void FSingleReportTransRotHidReader::ReadData(FTickArgs& output, float deltaSecs)
    {
        // TODO: debug
        // Output.Debug->AppendReport(Report, GetReportSize());
        int32 report = 0;
        for (int i = 0; i < GetReportCount(); i++)
        {
            const uint8 reportID = OutputBuffer[report];

            if (reportID == 1)
            {
                FTransRotReport tr = *reinterpret_cast<FTransRotReport*>(&OutputBuffer[report + 1]);

                if (!CheckAxes(tr.TransX, tr.TransY, tr.TransZ, tr.RotX, tr.RotY, tr.RotZ))
                    continue;

                float fx = static_cast<float>(tr.TransX) / GetAxisResolution();
                float fy = static_cast<float>(tr.TransY) / GetAxisResolution();
                float fz = static_cast<float>(tr.TransZ) / GetAxisResolution();
                float rfx = static_cast<float>(tr.RotX) / GetAxisResolution();
                float rfy = static_cast<float>(tr.RotY) / GetAxisResolution();
                float rfz = static_cast<float>(tr.RotZ) / GetAxisResolution();
                
                output.MovementState.FlagMove();

                ApplyTranslation(output, fx, fy, fz, deltaSecs);
                ApplyRotation(output, rfx, rfy, rfz, deltaSecs);

                // TODO: Debug
            }
            else if (reportID == 3 && ButtonSource == EButtonReportSource::ButtonBits_Report3)
            {
                uint32 buttonBits = *reinterpret_cast<uint32*>(&OutputBuffer[report + 1]);
                Buttons::ButtonBitsToQueue(buttonBits, output.ProcessedData.ButtonQueue);
                Buttons::ButtonBitsToQueue(buttonBits, output.NormData.ButtonQueue);
            }
            else if (reportID == 28 && ButtonSource == EButtonReportSource::ButtonQueue_Report28)
            {
                Buttons::FButtonQueue queue { .All = *reinterpret_cast<uint64*>(&OutputBuffer[report + 1]) };
                output.ProcessedData.ButtonQueue = queue;
                output.NormData.ButtonQueue = queue;
            }
            report += GetReportSize();
        }
    }

    Hid::FScopedHidDevice const& FSingleReportTransRotHidReader::GetDevice()
    {
        return Device;
    }
}
