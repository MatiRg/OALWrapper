/*
 * Copyright 2007-2010 (C) - Frictional Games
 *
 * This file is part of OALWrapper
 *
 * For conditions of distribution and use, see copyright notice in LICENSE
 */
#include "OALWrapper/OAL_LoggerObject.h"
#include "OALWrapper/OAL_Helper.h"
#include <codecvt>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <locale>

bool iOAL_LoggerObject::mbLogEnabled = false;
eOAL_LogVerbose iOAL_LoggerObject::mLogVerboseLevel = eOAL_LogVerbose_Low;
iOAL_LoggerSink* iOAL_LoggerObject::mpLogSink = nullptr;

void iOAL_LoggerObject::LogMsg(const std::string& asIDStr, eOAL_LogVerbose aVerbose, eOAL_LogMsg aType, const char* asMessage, ...)
{
    if (!mbLogEnabled)
        return;
    if (mLogVerboseLevel < aVerbose)
        return;
    if (!asMessage)
        return;

    std::string sMessage;

    char text[2048];
    va_list ap;
    va_start(ap, asMessage);
    vsprintf(text, asMessage, ap);
    va_end(ap);

    sMessage.append(asIDStr.c_str()).append(text);
    Write(aType, sMessage);
}

//---------------------------------------------------------------------------------------

void iOAL_LoggerObject::Write(eOAL_LogMsg aType, const std::string& asMessage)
{
    if (!mbLogEnabled)
    {
        return;
    }

	if(mpLogSink && !asMessage.empty())
	{
		mpLogSink->Write(aType, asMessage);
	}
}

//

void OAL_SetupLogging(bool abLogSounds, iOAL_LoggerSink* apLogSink, eOAL_LogVerbose aeVerboseLevel)
{
    iOAL_LoggerObject::SetLogEnabled(abLogSounds);
    iOAL_LoggerObject::SetLogVerbose(aeVerboseLevel);
	iOAL_LoggerObject::SetLogSink(apLogSink);
}

void OAL_Log(eOAL_LogVerbose aeVerboseLevelReq, eOAL_LogMsg aeMessageType, const char* asMessage, ...)
{
    if ( !asMessage )
        return;
    if (!iOAL_LoggerObject::IsLogEnabled())
        return;
    if (iOAL_LoggerObject::GetLogVerboseLevel() < aeVerboseLevelReq)
        return;

    std::string sMessage;

    char text[2048];
    va_list ap;
    va_start(ap, asMessage);
    vsprintf(text, asMessage, ap);
    va_end(ap);

    sMessage.append(text);
    iOAL_LoggerObject::Write(aeMessageType, sMessage);
}
