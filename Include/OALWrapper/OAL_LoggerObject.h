/*
 * Copyright 2007-2010 (C) - Frictional Games
 *
 * This file is part of OALWrapper
 *
 * For conditions of distribution and use, see copyright notice in LICENSE
 */
#pragma once
#include "OAL_Types.h"

enum eOAL_LogMsg
{
    eOAL_LogMsg_Text,
    eOAL_LogMsg_Command,
    eOAL_LogMsg_Info,
    eOAL_LogMsg_Error,
    eOAL_LogMsg_Default,
};

enum eOAL_LogVerbose
{
    eOAL_LogVerbose_None,
    eOAL_LogVerbose_Low,
    eOAL_LogVerbose_Medium,
    eOAL_LogVerbose_High,
    eOAL_LogVerbose_Default,
};

enum eOAL_LogOutput
{
    eOAL_LogOutput_File,
    eOAL_LogOutput_Console,
    eOAL_LogOutput_Default,
};

class iOAL_LoggerObject
{
public:
    iOAL_LoggerObject() = default;
    virtual ~iOAL_LoggerObject() = default;

    static void LogMsg(const std::string& asIDStr, eOAL_LogVerbose aVerbose, eOAL_LogMsg aType, const char* asMessage, ...);

    static void Write(const std::string& asMessage);

    static void SetLogEnabled(bool abEnable) { mbLogEnabled = abEnable; }
    static void SetLogOutput(eOAL_LogOutput aOutput) { mLogOutput = aOutput; }
    static void SetLogVerbose(eOAL_LogVerbose aLevel) { mLogVerboseLevel = aLevel; }
    static void SetLogFilename(const std::string& asLogFilename);

    static bool IsLogEnabled() { return mbLogEnabled; }
    static eOAL_LogOutput GetLogOutput() { return mLogOutput; }
    static eOAL_LogVerbose GetLogVerboseLevel() { return mLogVerboseLevel; }
    static std::wstring GetLogFilename() { return msLogFile; }

protected:
    static bool mbLogEnabled;
    static eOAL_LogOutput mLogOutput;
    static eOAL_LogVerbose mLogVerboseLevel;
    static std::wstring msLogFile;
};
