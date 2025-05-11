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




#include "ReadingMethod/SingleReportTransRotMethod.h"

#include "DebugInfoPrinter.h"
#include "MovementState.h"


FSingleReportTransRotMethod::FSingleReportTransRotMethod()
{
}

FSingleReportTransRotMethod::~FSingleReportTransRotMethod()
{
}

#define CHECK_AXES() \
    (  xx <= GetAxisResolution() \
    && yy <= GetAxisResolution() \
    && zz <= GetAxisResolution() \
    && rxx <= GetAxisResolution() \
    && ryy <= GetAxisResolution() \
    && rzz <= GetAxisResolution() \
    && -xx >= -GetAxisResolution() \
    && -yy >= -GetAxisResolution() \
    && -zz >= -GetAxisResolution() \
    && -rxx >= -GetAxisResolution() \
    && -ryy >= -GetAxisResolution() \
    && -rzz >= -GetAxisResolution() \
    )

void FSingleReportTransRotMethod::ReadData(FSmDataReadingOutput& Output, float DeltaSecs, uint8* Report)
{
    Output.Debug->AppendReport(Report, GetReportSize());
    for (int i = 0; i < GetReportCount(); i++)
    {
        const uint8 ReportID = *Report;
        /*int16 xx = *(pCurr + 1) | (uint16)(*(pCurr + 2)) << 8;
        int16 yy = *(pCurr + 3) | (uint16)(*(pCurr + 4)) << 8;
        int16 zz = *(pCurr + 5) | (uint16)(*(pCurr + 6)) << 8;*/

        int16 xx = *reinterpret_cast<int16*>(Report + 1);
        int16 yy = *reinterpret_cast<int16*>(Report + 3);
        int16 zz = *reinterpret_cast<int16*>(Report + 5);
        int16 rxx = *reinterpret_cast<int16*>(Report + 7);
        int16 ryy = *reinterpret_cast<int16*>(Report + 9);
        int16 rzz = *reinterpret_cast<int16*>(Report + 11);

        float fx = static_cast<float>(xx) / GetAxisResolution();
        float fy = static_cast<float>(yy) / GetAxisResolution();
        float fz = static_cast<float>(zz) / GetAxisResolution();
        float rfx = static_cast<float>(rxx) / GetAxisResolution();
        float rfy = static_cast<float>(ryy) / GetAxisResolution();
        float rfz = static_cast<float>(rzz) / GetAxisResolution();

        if (ReportID == 0 && Output.Settings.bPrintDebug)
        {
            Output.Debug->SetReport(ReportID, Report, GetReportSize());
        }

        if (ReportID == 1 && CHECK_AXES())
        {
            Output.MovementState->Move();

            ApplyTranslation(Output, fx, fy, fz, DeltaSecs);
            ApplyRotation(Output, rfx, rfy, rfz, DeltaSecs);

            Output.Debug->SetReport(ReportID, Report, GetReportSize());
        }
        else if (ReportID == 3)
        {
            ApplyButtons(Output, Report);
        }
        Report += GetReportSize();
    }
}

#undef CHECK_AXES
