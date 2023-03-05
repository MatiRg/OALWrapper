/*
 * Copyright 2007-2010 (C) - Frictional Games
 *
 * This file is part of OALWrapper
 *
 * For conditions of distribution and use, see copyright notice in LICENSE
 */
/**
    @file OAL_Playback.h
    @author Luis Rodero
    @date 2006-10-02
    @version 0.1
    Set of functions to manage playback and such in OpenAL
*/
#include "OALWrapper/OAL_Playback.h"
#include "OALWrapper/OAL_Device.h"
#include "OALWrapper/OAL_Source.h"
#include "OALWrapper/OAL_Types.h"

extern cOAL_Device* gpDevice;

//////////////////////////////////////////////////////////////////
////	int OAL_Sample_Play ( int alSource, const cOAL_Sample* apSample )
////	-	Begins play of a loaded Sample on the specified source. If alSource == OAL_FREE,
////		it looks for a free source if there is any, if not, it selects that which has the lowest
////		priority below the sample's own prio
//////////////////////////////////////////////////////////////////

int OAL_Sample_Play(int alSource, cOAL_Sample* apSample, float afVolume, bool abStartPaused,
                    int alPriority)
{
    if (gpDevice == nullptr)
        return -1;
    return gpDevice->PlaySample(alSource, apSample, alPriority, afVolume, abStartPaused);
}

//////////////////////////////////////////////////////////////////
////	int OAL_Stream_Play ( int alSource, const cOAL_Stream* apStream, float afVolume, bool abStartPaused )
////	-	Begins play of an open Stream on the specified source. If alSource == OAL_FREE,
////		it looks for a free source if there is any, if not, it selects the source with lowest prio
//////////////////////////////////////////////////////////////////

int OAL_Stream_Play(int alSource, cOAL_Stream* apStream, float afVolume, bool abStartPaused)
{
    if (gpDevice == nullptr)
        return -1;
    return gpDevice->PlayStream(alSource, apStream, afVolume, abStartPaused);
}

void OAL_Source_Stop_All()
{
    if (gpDevice == nullptr)
    {
        return;
    }
	if (gpDevice->GetSourceManager() == nullptr)
    {
        return;
    }
	for(const auto& i: gpDevice->GetSourceManager()->GetSources())
	{
		if (i)
		{
			i->Lock();
			i->Stop();
			i->Unlock();
		}
	}
}

//////////////////////////////////////////////////////////////////
////	void OAL_Source_Stop ( int alSource )
////	-	Stops the specified source.
//////////////////////////////////////////////////////////////////

void OAL_Source_Stop(int alSource)
{
    if (gpDevice == nullptr)
        return;
    cOAL_Source* pSource = gpDevice->GetSource(alSource);

    if (pSource)
    {
        pSource->Lock();
        pSource->Stop();
        pSource->Unlock();
    }
}

///////////////////////////////////////////////////////////
////	void OAL_Source_SetGain ( int alSource, float afVolume )
////	-	Sets the volume for the specified Source
///////////////////////////////////////////////////////////

void OAL_Source_SetGain(int alSource, float afVolume)
{
    if (gpDevice == nullptr)
        return;
    cOAL_Source* pSource = gpDevice->GetSource(alSource);
    if (pSource)
    {
        pSource->Lock();
        pSource->SetGain(afVolume);
        pSource->Unlock();
    }
}
//////////////////////////////////////////////////////////////////
////	void OAL_Source_SetPitch ( int alSource, const float afPitch )
////	-	Sets pitch value for specified source
//////////////////////////////////////////////////////////////////

void OAL_Source_SetPitch(int alSource, float afPitch)
{
    if (gpDevice == nullptr)
        return;
    cOAL_Source* pSource = gpDevice->GetSource(alSource);
    if (pSource)
    {
        pSource->Lock();
        pSource->SetPitch(afPitch);
        pSource->Unlock();
    }
}

//////////////////////////////////////////////////////////////////
////	void OAL_Source_SetPaused ( int alSource, bool abPaused )
////	-	Pauses the specified source.
//////////////////////////////////////////////////////////////////

void OAL_Source_SetPaused(int alSource, bool abPaused)
{
    if (gpDevice == nullptr)
        return;
    cOAL_Source* pSource = gpDevice->GetSource(alSource);
    if (pSource)
    {
        pSource->Lock();
        pSource->Pause(abPaused);
        pSource->Unlock();
    }
}

//////////////////////////////////////////////////////////////////
////	void OAL_Source_SetLoop ( int alSource, bool abPaused )
////	-	Pauses the specified source.
//////////////////////////////////////////////////////////////////

void OAL_Source_SetLoop(int alSource, bool abLoop)
{
    if (gpDevice == nullptr)
        return;
    cOAL_Source* pSource = gpDevice->GetSource(alSource);
    if (pSource)
    {
        pSource->Lock();
        pSource->SetLoop(abLoop);
        pSource->Unlock();
    }
}

//////////////////////////////////////////////////////////////////
////	void OAL_Source_Position ( const int alSource, const float* apPos )
////	-	Sets 3D audio position for the specified source.
//////////////////////////////////////////////////////////////////

void OAL_Source_SetPosition(const int alSource, const float* apPos)
{
    if (gpDevice == nullptr)
        return;
    cOAL_Source* pSource = gpDevice->GetSource(alSource);
    if (pSource)
    {
        pSource->Lock();
        pSource->SetPosition(apPos);
        pSource->Unlock();
    }
}

//////////////////////////////////////////////////////////////////
////	void OAL_Source_SetVelocity ( const int alSource, const float* apVel )
////	-	Sets 3D audio velocity for the specified source.
//////////////////////////////////////////////////////////////////

void OAL_Source_SetVelocity(const int alSource, const float* apVel)
{
    if (gpDevice == nullptr)
        return;
    cOAL_Source* pSource = gpDevice->GetSource(alSource);
    if (pSource)
    {
        pSource->Lock();
        pSource->SetVelocity(apVel);
        pSource->Unlock();
    }
}

//////////////////////////////////////////////////////////////////
////	void OAL_Source_SetAttributes ( const int alSource, const float* apPos, const float* apVel )
////	-	Sets 3D audio attributes for the specified source.
//////////////////////////////////////////////////////////////////

void OAL_Source_SetAttributes(const int alSource, const float* apPos, const float* apVel)
{
    if (gpDevice == nullptr)
        return;
    cOAL_Source* pSource = gpDevice->GetSource(alSource);
    if (pSource)
    {
        pSource->Lock();
        pSource->SetPosition(apPos);
        pSource->SetVelocity(apVel);
        pSource->Unlock();
    }
}

void OAL_Source_SetMinDistance(const int alSource, const float afMin)
{
    if (gpDevice == nullptr)
        return;
    cOAL_Source* pSource = gpDevice->GetSource(alSource);
    if (pSource)
    {
        pSource->Lock();
        pSource->SetMinDistance(afMin);
        pSource->Unlock();
    }
}

void OAL_Source_SetMaxDistance(const int alSource, const float afMax)
{
    if (gpDevice == nullptr)
        return;
    cOAL_Source* pSource = gpDevice->GetSource(alSource);
    if (pSource)
    {
        pSource->Lock();
        pSource->SetMaxDistance(afMax);
        pSource->Unlock();
    }
}

void OAL_Source_SetAttenuation(const int alSource, const float afAttenuation)
{
    if (gpDevice == nullptr)
        return;
    cOAL_Source* pSource = gpDevice->GetSource(alSource);
    if (pSource)
    {
        pSource->Lock();
        pSource->SetAttenuation(afAttenuation);
        pSource->Unlock();
    }
}

//////////////////////////////////////////////////////////////////
////	void OAL_Source_SetPositionRelative ( const int alSource, const bool abRelative )
////	-	Sets if the specified source should use coordinates relative to the Listener.
//////////////////////////////////////////////////////////////////

void OAL_Source_SetPositionRelative(const int alSource, const bool abRelative)
{
    if (gpDevice == nullptr)
        return;
    cOAL_Source* pSource = gpDevice->GetSource(alSource);
    if (pSource)
    {
        pSource->Lock();
        pSource->SetPositionRelative(abRelative);
        pSource->Unlock();
    }
}

//////////////////////////////////////////////////////////////////
////	void OAL_Listener_SetAttributes ( const float* apPos, const float* apVel, const float* apForward, const float* apUp )
////	-	Sets 3D audio attributes for the listener.
//////////////////////////////////////////////////////////////////

void OAL_Listener_SetAttributes(const float* apPos, const float* apVel, const float* apForward, const float* apUp)
{
    if (gpDevice == nullptr)
        return;
    if (apPos)
        gpDevice->SetListenerPosition(apPos);
    if (apVel)
        gpDevice->SetListenerVelocity(apVel);
    if (apForward && apUp)
        gpDevice->SetListenerOrientation(apForward, apUp);
}

///////////////////////////////////////////////////////////
////	void OAL_Listener_SetMasterVolume ( const float afVolume )
////	-	Sets the master volume
///////////////////////////////////////////////////////////

void OAL_Listener_SetMasterVolume(float afVolume)
{
    if (gpDevice == nullptr)
        return;
    gpDevice->SetListenerGain(afVolume);
}

///////////////////////////////////////////////////////////
////	void OAL_Source_SetPriority ( const int alSource, const unsigned int alPriority )
////	-	Sets the priority of the specified source
///////////////////////////////////////////////////////////

void OAL_Source_SetPriority(const int alSource, const unsigned int alPriority)
{
    if (gpDevice == nullptr)
        return;
    cOAL_Source* pSource = gpDevice->GetSource(alSource);
    if (pSource)
    {
        pSource->Lock();
        pSource->SetPriority(alPriority);
        pSource->Unlock();
    }
}

///////////////////////////////////////////////////////////
////	const unsigned int OAL_Source_GetPriority ( const int alSource )
////	-	Returns the source priority
///////////////////////////////////////////////////////////

unsigned int OAL_Source_GetPriority(const int alSource)
{
    if (gpDevice == nullptr)
        return 0;
    int lPriority = 0;
    cOAL_Source* pSource = gpDevice->GetSource(alSource);
    if (pSource)
    {
        pSource->Lock();
        lPriority = pSource->GetPriority();
        pSource->Unlock();
    }

    return lPriority;
}

///////////////////////////////////////////////////////////
////	bool OAL_Source_IsPlaying ( const int alSource )
////	-	Returns true if the specified source is playing audio
///////////////////////////////////////////////////////////

bool OAL_Source_IsPlaying(const int alSource)
{
    if (gpDevice == nullptr)
        return false;
    bool bIsPlaying = false;

    cOAL_Source* pSource = gpDevice->GetSource(alSource);
    if (pSource)
    {
        pSource->Lock();
        eOAL_SourceStatus eStatus = pSource->GetSourceStatus();
        pSource->Unlock();

        bIsPlaying = (eStatus != eOAL_SourceStatus_Free);
    }
    return bIsPlaying;
}

bool OAL_Source_IsBufferUnderrun(const int)
{
    return false;
}

void OAL_Source_SetElapsedTime(const int alSource, double afTime)
{
    if (gpDevice == nullptr)
        return;

    cOAL_Source* pSource = gpDevice->GetSource(alSource);
    if (pSource)
    {
        pSource->Lock();
        pSource->SetElapsedTime(afTime);
        pSource->Unlock();
    }
}

double OAL_Source_GetElapsedTime(const int alSource)
{
    if (gpDevice == nullptr)
        return 0;
    double fElapsedTime = 0;
    cOAL_Source* pSource = gpDevice->GetSource(alSource);
    if (pSource)
    {
        pSource->Lock();
        fElapsedTime = pSource->GetElapsedTime();
        pSource->Unlock();
    }

    return fElapsedTime;
}

double OAL_Source_GetTotalTime(const int alSource)
{
    if (gpDevice == nullptr)
        return 0;
    double fTotalTime = 0;
    cOAL_Source* pSource = gpDevice->GetSource(alSource);
    if (pSource)
    {
        pSource->Lock();
        fTotalTime = pSource->GetTotalTime();
        pSource->Unlock();
    }

    return fTotalTime;
}

float OAL_Source_GetGain(const int alSource)
{
    if (gpDevice == nullptr)
        return 0;
    float fGain = 0.0f;
    cOAL_Source* pSource = gpDevice->GetSource(alSource);
    if (pSource)
    {
        pSource->Lock();
        fGain = pSource->GetGain();
        pSource->Unlock();
    }
    return fGain;
}

float OAL_Source_GetPitch(const int alSource)
{
    if (gpDevice == nullptr)
        return 0;
    float fPitch = 0.0f;
    cOAL_Source* pSource = gpDevice->GetSource(alSource);
    if (pSource)
    {
        pSource->Lock();
        fPitch = pSource->GetPitch();
        pSource->Unlock();
    }
    return fPitch;
}
