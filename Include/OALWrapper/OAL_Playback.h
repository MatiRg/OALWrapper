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
#pragma once
#include "OAL_Types.h"
#include <string>

int OAL_Sample_Play(int alSource, cOAL_Sample* apSample, float afVolume, bool abStartPaused, int alPriority);
int OAL_Stream_Play(int alSource, cOAL_Stream* apStream, float afVolume, bool abStartPaused);

void OAL_Source_Stop_All();
void OAL_Source_Stop(int alSource);
void OAL_Source_SetPaused(int alSource, const bool abPaused);
void OAL_Source_SetGain(int alSource, float afVolume);
void OAL_Source_SetPitch(int alSource, float afPitch);
void OAL_Source_SetLoop(int alSource, const bool abLoop);
void OAL_Source_SetPosition(const int alSource, const float* apPos);
void OAL_Source_SetVelocity(const int alSource, const float* apVel);
void OAL_Source_SetAttributes(const int alSource, const float* apPos, const float* apVel);

void OAL_Source_SetMinDistance(const int alSource, const float afMin);
void OAL_Source_SetMaxDistance(const int alSource, const float afMax);
void OAL_Source_SetAttenuation(const int alSource, const float afAttenuation);

void OAL_Source_SetPositionRelative(const int alSource, const bool abRelative);
void OAL_Source_SetPriority(const int alSource, const unsigned int alPriority);

unsigned int OAL_Source_GetPriority(const int alSource);
float OAL_Source_GetPitch(const int alSource);
float OAL_Source_GetGain(const int alSource);
bool OAL_Source_IsPlaying(const int alSource);
bool OAL_Source_IsBufferUnderrun(const int alSource);

void OAL_Listener_SetAttributes(const float* apPos, const float* apVel, const float* apForward, const float* apUpward);
void OAL_Listener_SetMasterVolume(const float afVolume);

void OAL_Source_SetElapsedTime(const int alSource, double afTime);
double OAL_Source_GetElapsedTime(const int alSource);
double OAL_Source_GetTotalTime(const int alSource);
