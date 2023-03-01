/*
 * Copyright 2007-2010 (C) - Frictional Games
 *
 * This file is part of OALWrapper
 *
 * For conditions of distribution and use, see copyright notice in LICENSE
 */
/**
	@file OAL_Init.cpp
	@author Luis Rodero
	@date 2006-10-10
	@version 0.1
	Set of functions for Initialization/Shutdown, plus misc funcs
*/

#include "OALWrapper/OAL_Init.h"
#include "OALWrapper/OAL_Types.h"
#include "OALWrapper/OAL_Device.h"
#include "OALWrapper/OAL_SourceManager.h"
#include "OALWrapper/OAL_Stream.h"

#include <cstdarg>
#include <cstdlib>
#include <cstdio>

using namespace std;

cOAL_Device* gpDevice = NULL;



///////////////////////////////////////////////////////////
//	bool OAL_Init ( char *asDeviceName,	int alOutputFreq,
//					bool abUseThread, 	int alUpdateFreq,
//					int alReqMajorVersion, int alReqMinorVersion)
///////////////////////////////////////////////////////////
bool	OAL_Init ( cOAL_Init_Params &acParams )
{
	OAL_Log(eOAL_LogVerbose_None, eOAL_LogMsg_Text,"-------------------------------------\n----- OpenAL Wrapper Log opened -----\n-------------------------------------\n");

	gpDevice = new cOAL_Device;
	bool bSuccess = gpDevice->Init( acParams );
	if ( bSuccess )
	{
		atexit(OAL_Close);
		return true;
	}

	OAL_Close();
	return false;
}

///////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////

void	OAL_Close ()
{
	if ( gpDevice == NULL )
		return;

	gpDevice->SetListenerGain ( 0.0f );

//    OAL_Source_Stop ( OAL_ALL );

	gpDevice->Close();
	delete gpDevice;
	gpDevice = NULL;

    OAL_Log(eOAL_LogVerbose_Low, eOAL_LogMsg_Text,"-------------------------------------\n----- OpenAL Wrapper Log closed -----\n-------------------------------------\n");
}

///////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////

void	OAL_Update ()
{
	if (gpDevice == NULL)
		return;

	if ( gpDevice->GetSourceManager()->IsThreadAlive() )
		return;

	gpDevice->GetSourceManager()->UpdateStreaming();
}

///////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////

void	OAL_SetRollOffFactor ( const float )
{
	/*
	DEF_FUNC_NAME("OAL_SetRollOffFactor");
	FUNC_USES_AL;
	for ( unsigned int i = 0; i < gpSourceList->size(); ++i )
	{
		alSourcef ( (*gpSourceList)[i]->mlSource, AL_ROLLOFF_FACTOR, afFactor );
		AL_CHECK_CALL("alSourcef");
	}
	*/
}

///////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////

void	OAL_SetDistanceModel ( eOAL_DistanceModel aeModel )
{
	if (gpDevice == NULL)
		return;
	gpDevice->SetDistanceModel(aeModel);
}




///////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////
/*
*/
void OAL_LogSourcePoolStatus ()
{
}

void OAL_SetupLogging ( bool abLogSounds, eOAL_LogOutput aeOutput, eOAL_LogVerbose aeVerboseLevel, string asLogFilename )
{
	iOAL_LoggerObject::SetLogEnabled(abLogSounds);
	iOAL_LoggerObject::SetLogOutput ( aeOutput );
	iOAL_LoggerObject::SetLogVerbose( aeVerboseLevel );
	iOAL_LoggerObject::SetLogFilename( asLogFilename );

	FILE* pTempFile = OpenFileW(iOAL_LoggerObject::GetLogFilename(), L"a");

	if (pTempFile)
	{
		fclose(pTempFile);
#ifdef WIN32
		_wremove (iOAL_LoggerObject::GetLogFilename().c_str());
#else
		remove (WString2String(iOAL_LoggerObject::GetLogFilename()).c_str());
#endif
	}

	pTempFile = NULL;
/*
	for (int i = 0; i < 4096; ++i)
	{
		char buffer[100];

		sprintf(buffer, "./OAL/OAL_Source_%d.log", i);
		pTempFile = OpenFile(string(buffer),"r");
		if (pTempFile)
		{
			fclose(pTempFile);
			remove(buffer);
		}
		else
			break;
	}
*/
}

void OAL_Log ( eOAL_LogVerbose aeVerboseLevelReq, eOAL_LogMsg aeMessageType,  const char* asMessage, ... )
{
	if (asMessage == NULL)
		return;
	if (!iOAL_LoggerObject::IsLogEnabled())
		return;

	if (iOAL_LoggerObject::GetLogVerboseLevel() < aeVerboseLevelReq )
		return;

	string sMessage;

	char text[2048];
	va_list ap;
	va_start(ap, asMessage);
	vsprintf(text, asMessage, ap);
	va_end(ap);


	switch ( aeMessageType )
	{
	case eOAL_LogMsg_Command:
		sMessage.append("[COMMAND] ");
		break;
	case eOAL_LogMsg_Info:
		sMessage.append("[INFO] ");
		break;
	case eOAL_LogMsg_Error:
		sMessage.append("[ERROR] ");
	default:
		break;
	}

	sMessage.append(text);

	iOAL_LoggerObject::Write(sMessage);
}

const char* OAL_Info_GetDeviceName()
{
	if (gpDevice)
		return gpDevice->GetDeviceName().c_str();
	else
		return NULL;
}

const char* OAL_Info_GetVendorName()
{
	if (gpDevice)
		return gpDevice->GetVendorName().c_str();
	else
		return NULL;
}

const char* OAL_Info_GetRendererName()
{
	if (gpDevice)
		return gpDevice->GetRenderer().c_str();
	else
		return NULL;
}

int		OAL_Info_GetMajorVersion()
{
	if (gpDevice)
		return gpDevice->GetMajorVersion();
	else
		return 0;
}

int		OAL_Info_GetMinorVersion()
{
	if (gpDevice)
		return gpDevice->GetMinorVersion();
	else
		return 0;
}

int	OAL_Info_GetNumSources()
{
	if (gpDevice)
		return gpDevice->GetNumSources();
	else
		return 0;
}

bool	OAL_Info_IsEFXActive()
{
	if (gpDevice)
		return gpDevice->IsEFXActive();
	else
		return false;
}

int OAL_Info_GetStreamBufferCount()
{
	return cOAL_Stream::GetBufferCount();
}

int OAL_Info_GetStreamBufferSize()
{
	return cOAL_Stream::GetBufferSize();
}

string OAL_Info_GetDefaultOutputDevice()
{
	return cOAL_Device::GetDefaultDeviceName();
}

vector<string> OAL_Info_GetOutputDevices()
{
    return cOAL_Device::GetOutputDevices();
}
