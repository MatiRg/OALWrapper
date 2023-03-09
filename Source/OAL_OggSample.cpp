/*
 * Copyright 2007-2010 (C) - Frictional Games
 *
 * This file is part of OALWrapper
 *
 * For conditions of distribution and use, see copyright notice in LICENSE
 */
/**
    @file OAL_OggSample.cpp
    @author Luis Rodero
    @date 2006-10-02
    @version 0.1
    Derived class for containing Ogg Vorbis Sample data
*/
#include "OALWrapper/OAL_OggSample.h"
#include "OALWrapper/OAL_Buffer.h"
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#define STB_VORBIS_HEADER_ONLY
#include <stb_vorbis.c>

bool cOAL_OggSample::CreateInternal(stb_vorbis* STBFile)
{
    if( !STBFile )
    {
        return false;
    }
    stb_vorbis_info Info = stb_vorbis_get_info( STBFile );
    if( !Info.channels || Info.channels > 2 )
    {
        return false;
    }
    mlChannels = Info.channels;
    mFormat = (mlChannels == 2) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
    mlFrequency = Info.sample_rate;
    mlSamples = stb_vorbis_stream_length_in_samples(STBFile)*Info.channels;
    mfTotalTime = static_cast<float>(mlSamples) / static_cast<float>(mlChannels) / static_cast<float>(mlFrequency);

    std::vector<ALshort> Data(mlSamples);
    stb_vorbis_get_samples_short_interleaved(STBFile, Info.channels, Data.data(), mlSamples);

    cOAL_Buffer* pBuffer = mvBuffers[0];
    mbStatus = pBuffer->Feed(Data.data(), mlSamples*GetBytesPerSample());

    return mbStatus;
}

bool cOAL_OggSample::CreateFromFile(const std::wstring& asFilename)
{
    msFilename = asFilename;
    std::string FileName = WString2String(msFilename);
    stb_vorbis* STBFile = stb_vorbis_open_filename(FileName.c_str(), nullptr, nullptr);
    const bool Status = CreateInternal(STBFile);
    stb_vorbis_close(STBFile);
    return Status;
}

bool cOAL_OggSample::CreateFromBuffer(const void* apBuffer, size_t aSize)
{
    stb_vorbis* STBFile = stb_vorbis_open_memory(static_cast<const unsigned char*>(apBuffer), aSize, nullptr, nullptr);
    const bool Status = CreateInternal(STBFile);
    stb_vorbis_close(STBFile);
    return Status;
}
