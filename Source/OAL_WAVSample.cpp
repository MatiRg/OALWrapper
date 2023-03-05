/*
 * Copyright 2007-2010 (C) - Frictional Games
 *
 * This file is part of OALWrapper
 *
 * For conditions of distribution and use, see copyright notice in LICENSE
 */
/**
    @file OAL_WAVSample.cpp
    @author Luis Rodero
    @date 2006-10-02
    @version 0.1
    Derived class for containing WAV Sample data
*/
#include "OALWrapper/OAL_WAVSample.h"
#include "OALWrapper/OAL_Buffer.h"
#include "OALWrapper/OAL_Device.h"
#include "OALWrapper/OAL_Helper.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <SDL2/SDL.h>

bool IsAudioFormat8(const SDL_AudioFormat x)
{
    if(x == AUDIO_S8 || x == AUDIO_U8 )
    {
        return true;
    }
    return false;
}

bool IsAudioFormat16(const SDL_AudioFormat x)
{
    if(x == AUDIO_U16 || x == AUDIO_S16 )
    {
        return true;
    }
    return false;
}

bool cOAL_WAVSample::SetInternalFormat(const SDL_AudioSpec& AudioSpec, size_t Length)
{
    mlChannels = AudioSpec.channels;
    if (mlChannels == 2)
    {
        if ( IsAudioFormat8(AudioSpec.format) )
        {
            mFormat = AL_FORMAT_STEREO8;
            mlSamples = Length / 2;
        }
        else if ( IsAudioFormat16(AudioSpec.format) )
        {
            mlSamples = Length / 4;
            mFormat = AL_FORMAT_STEREO16;
        }
        else
        {
            return false;
        }
    }
    else if (mlChannels == 1)
    {
        if ( IsAudioFormat8(AudioSpec.format) )
        {
            mlSamples = Length;
            mFormat = AL_FORMAT_MONO8;
        }
        else if ( IsAudioFormat16(AudioSpec.format) )
        {
            mlSamples = Length / 2;
            mFormat = AL_FORMAT_MONO16;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
    return true;
}

bool cOAL_WAVSample::CreateInternal(SDL_RWops* RWops)
{
    Uint8* pData = {};
    Uint32 Length = {};
    SDL_AudioSpec AudioSpec;
    if( !SDL_LoadWAV_RW(RWops, 1, &AudioSpec, &pData, &Length) )
    {
        return false;
    }
    //
    if( !SetInternalFormat(AudioSpec, Length) )
    {
        SDL_FreeWAV(pData);
        return false;
    }
    mlFrequency = AudioSpec.freq;
    mfTotalTime = static_cast<float>(mlSamples) / static_cast<float>(mlFrequency);

    cOAL_Buffer* pBuffer = mvBuffers[0];
    mbStatus = pBuffer->Feed(pData, Length);
    SDL_FreeWAV(pData);

    return mbStatus;
}

bool cOAL_WAVSample::CreateFromFile(const std::wstring& asFilename)
{
    std::string FileName = WString2String(asFilename);
    return CreateInternal(SDL_RWFromFile(FileName.c_str(), "rb"));
}

bool cOAL_WAVSample::CreateFromBuffer(const void* apBuffer, size_t aSize)
{
    return CreateInternal(SDL_RWFromConstMem(apBuffer, aSize));
}
