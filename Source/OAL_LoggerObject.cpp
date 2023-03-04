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
eOAL_LogOutput iOAL_LoggerObject::mLogOutput = eOAL_LogOutput_File;
eOAL_LogVerbose iOAL_LoggerObject::mLogVerboseLevel = eOAL_LogVerbose_Low;
std::wstring iOAL_LoggerObject::msLogFile = L"OAL.log";

//---------------------------------------------------------------------------------------

void iOAL_LoggerObject::SetLogFilename(const std::string& asLogFilename)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
    msLogFile = myconv.from_bytes(asLogFilename);
}

//---------------------------------------------------------------------------------------

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

    switch (aType)
    {
    case eOAL_LogMsg_Command:
        sMessage.append("[COMMAND] ");
        break;
    case eOAL_LogMsg_Info:
        sMessage.append("[INFO] ");
        break;
    case eOAL_LogMsg_Error:
        sMessage.append("[ERROR] ");
        break;
    case eOAL_LogMsg_Text:
        [[fallthrough]];
    case eOAL_LogMsg_Default:
        [[fallthrough]];
    default:
        break;
    }

    sMessage.append(asIDStr.c_str()).append(text);

    Write(sMessage);
}

//---------------------------------------------------------------------------------------

void iOAL_LoggerObject::Write(const std::string& asMessage)
{
    if (!mbLogEnabled)
        return;

    FILE* fLog = nullptr;

    switch (mLogOutput)
    {
    case eOAL_LogOutput_File:
        fLog = OpenFileW(msLogFile, L"a");
        if ( fLog )
        {
            fwrite(asMessage.c_str(), sizeof(char), asMessage.size(), fLog);
            fclose(fLog);
        }
        break;
    case eOAL_LogOutput_Console:
        printf("%s", asMessage.c_str());
        break;
    default:
        break;
    }
}
