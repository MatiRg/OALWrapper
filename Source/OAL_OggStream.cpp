/*
 * Copyright 2007-2010 (C) - Frictional Games
 *
 * This file is part of OALWrapper
 *
 * For conditions of distribution and use, see copyright notice in LICENSE
 */
/**
    @file OAL_OggStream.cpp
    @author Luis Rodero
    @date 2006-10-06
    @version 0.1
    Definition of Base class for containing Ogg Vorbis streams
*/
#include "OALWrapper/OAL_OggStream.h"
#include "OALWrapper/OAL_Buffer.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#define STB_VORBIS_HEADER_ONLY
#include <stb_vorbis.c>

cOAL_OggStream::cOAL_OggStream()
{
}

cOAL_OggStream::~cOAL_OggStream()
{
    Destroy();
}

bool cOAL_OggStream::Stream(cOAL_Buffer* apDestBuffer)
{
    long lDataSize = 0;

    double fStartTime = GetTime();

    // Loop which loads chunks of decoded data into a buffer
    while (lDataSize < (int)mlBufferSize)
    {
        long lSamples = stb_vorbis_get_samples_short_interleaved
        (
            STBFile, 
            mlChannels, 
            reinterpret_cast<short*>(mpPCMBuffer+lDataSize), 
            (mlBufferSize-lDataSize)/GetBytesPerSample()
        );

        lSamples *= mlChannels;
        mfActualTime += (static_cast<float>(lSamples) / static_cast<float>(mlChannels) / static_cast<float>(mlFrequency));
        const long lChunkSize = lSamples*GetBytesPerSample();

        // If we get a 0, then we are at the end of the file
        if (lChunkSize == 0)
        {
            break;
        }
        else
            lDataSize += lChunkSize;
    }
    // Bind the data to the Buffer Object
    if (lDataSize)
        mbStatus = apDestBuffer->Feed(mpPCMBuffer, lDataSize, fStartTime);
    else
        mbEOF = true;

    return (lDataSize != 0);
}

void cOAL_OggStream::Seek(float afWhere, bool abForceRebuffer)
{
    mbEOF = false;
    if (afWhere < 0.0f)
    {
        afWhere = 0.0f;
    }
    if (afWhere > 1.0f)
    {
        afWhere = 1.0f;
    }
    afWhere *= mfTotalTime;

    const int SampleOffset = static_cast<int>(static_cast<float>(mlFrequency)*afWhere);
    mfActualTime = static_cast<float>(SampleOffset) / static_cast<float>(mlChannels) / static_cast<float>(mlFrequency);
    stb_vorbis_seek(STBFile, SampleOffset);

    if (abForceRebuffer)
    {
        mbNeedsRebuffering = true;
    }
}

double cOAL_OggStream::GetTime()
{
    return mfActualTime;
}

bool cOAL_OggStream::CreateInternal()
{
    if( !STBFile )
    {
        return false;
    }
    stb_vorbis_info Info = stb_vorbis_get_info( STBFile );
    if( !Info.channels || Info.channels > 2 )
    {
        Destroy();
        return false;
    }
    mlChannels = Info.channels;
    mFormat = (mlChannels == 2) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
    mlFrequency = Info.sample_rate;
    mlSamples = stb_vorbis_stream_length_in_samples(STBFile)*Info.channels;
    mfTotalTime = static_cast<float>(mlSamples) / static_cast<float>(mlChannels) / static_cast<float>(mlFrequency);
    mbStatus = true;

    return mbStatus;
}

bool cOAL_OggStream::CreateFromFile(const std::wstring& asFilename)
{
    msFilename = asFilename;
    std::string FileName = WString2String(msFilename);
    STBFile = stb_vorbis_open_filename(FileName.c_str(), nullptr, nullptr);
    return CreateInternal();
}

bool cOAL_OggStream::CreateFromBuffer(const void* apBuffer, size_t aSize)
{
    STBFile = stb_vorbis_open_memory(static_cast<const unsigned char*>(apBuffer), aSize, nullptr, nullptr);
    return CreateInternal();
}

void cOAL_OggStream::Destroy()
{
    if (STBFile)
    {
        stb_vorbis_close(STBFile);
        STBFile = nullptr;
    }
}
