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

class iOAL_LoggerSink
{
public:
    iOAL_LoggerSink() = default;
    virtual ~iOAL_LoggerSink() = default;

	virtual void Write(eOAL_LogMsg aType, const std::string& asMessage) = 0;
};

class iOAL_LoggerObject
{
public:
    iOAL_LoggerObject() = default;
    virtual ~iOAL_LoggerObject() = default;

    static void LogMsg(const std::string& asIDStr, eOAL_LogVerbose aVerbose, eOAL_LogMsg aType, const char* asMessage, ...);

    static void Write(eOAL_LogMsg aType, const std::string& asMessage);

    static void SetLogEnabled(bool abEnable) { mbLogEnabled = abEnable; }
    static void SetLogVerbose(eOAL_LogVerbose aLevel) { mLogVerboseLevel = aLevel; }
	static void SetLogSink(iOAL_LoggerSink* apLogSink) { mpLogSink = apLogSink; }

    static bool IsLogEnabled() { return mbLogEnabled; }
    static eOAL_LogVerbose GetLogVerboseLevel() { return mLogVerboseLevel; }
	static iOAL_LoggerSink* GetLogSink() { return mpLogSink; }
protected:
    static bool mbLogEnabled;
    static eOAL_LogVerbose mLogVerboseLevel;
	static iOAL_LoggerSink* mpLogSink;
};

void OAL_SetupLogging(bool abLogSounds, iOAL_LoggerSink* apLogSink, eOAL_LogVerbose aVerbose = eOAL_LogVerbose_Low);
void OAL_Log(eOAL_LogVerbose aVerboseLevelReq, eOAL_LogMsg aMsg, const char* asMessage, ...);
