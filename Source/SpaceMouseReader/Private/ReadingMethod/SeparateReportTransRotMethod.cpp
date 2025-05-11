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




#include "ReadingMethod/SeparateReportTransRotMethod.h"

#include "DebugInfoPrinter.h"
#include "MovementState.h"


FSeparateReportTransRotMethod::FSeparateReportTransRotMethod()
{
}

FSeparateReportTransRotMethod::~FSeparateReportTransRotMethod()
{
}

#define CHECK_AXES() \
    (  xx <= GetAxisResolution() \
    && yy <= GetAxisResolution() \
    && zz <= GetAxisResolution() \
    && -xx >= -GetAxisResolution() \
    && -yy >= -GetAxisResolution() \
    && -zz >= -GetAxisResolution() \
    )

void FSeparateReportTransRotMethod::ReadData(FSmDataReadingOutput& Output, float DeltaSecs, uint8* Report)
{
    Output.Debug->AppendReport(Report, GetReportSize());
    
    for (int i = 0; i < GetReportCount(); i++)
    {
        const uint8 ReportID = *Report;

        int16 xx = *reinterpret_cast<int16*>(Report + 1);
        int16 yy = *reinterpret_cast<int16*>(Report + 3);
        int16 zz = *reinterpret_cast<int16*>(Report + 5);

        float fx = static_cast<float>(xx) / GetAxisResolution();
        float fy = static_cast<float>(yy) / GetAxisResolution();
        float fz = static_cast<float>(zz) / GetAxisResolution();

        if (ReportID == 0 && Output.Settings.bPrintDebug)
        {
            Output.Debug->SetReport(ReportID, Report, GetReportSize());
        }

        if (ReportID == 1 && CHECK_AXES())
        {
            ApplyTranslation(Output, fx, fy, fz, DeltaSecs);
            Output.Debug->SetReport(ReportID, Report, GetReportSize());
            Output.MovementState->Move();
        }
        if (ReportID == 2 && CHECK_AXES())
        {
            ApplyRotation(Output, fx, fy, fz, DeltaSecs);
            Output.Debug->SetReport(ReportID, Report, GetReportSize());
            Output.MovementState->Move();
        }
        if (ReportID == 3)
        {
            ApplyButtons(Output, Report);
        }
        Report += GetReportSize();
    }
}

#undef CHECK_AXES
