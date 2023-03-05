/*
 * Copyright 2007-2010 (C) - Frictional Games
 *
 * This file is part of OALWrapper
 *
 * For conditions of distribution and use, see copyright notice in LICENSE
 */
#pragma once
#include "OAL_Types.h"

class cOAL_Buffer;
class cOAL_Source;

enum eOAL_AudioDataType
{
    eOAL_AudioDataType_None,
    eOAL_AudioDataType_Sample,
    eOAL_AudioDataType_Stream,
};

class iOAL_AudioData
{
    friend class cOAL_Buffer;

public:
    iOAL_AudioData(eOAL_AudioDataType aType, int alBuffersToCreate);
    virtual ~iOAL_AudioData();

    virtual void Reset();
    virtual bool CreateFromFile(const std::wstring& asFilename) = 0;
    virtual bool CreateFromBuffer(const void* apBuffer, size_t aSize) = 0;
    virtual void Destroy() = 0;

    eOAL_AudioDataType GetType() { return mType; }
    std::wstring& GetFilename() { return msFilename; }

    ALint GetChannels() { return mlChannels; }
    ALenum GetFormat() { return mFormat; }
    ALint GetFrequency() { return mlFrequency; }
    double GetTotalTime() { return mfTotalTime; }
    bool GetStatus() { return mbStatus; }
    int GetBytesPerSample();
    virtual double GetProcessedBuffersTime() = 0;

    void SetLoop(bool abLoop) { mbLoop = abLoop; }

    const tBufferVec& GetBuffers() { return mvBuffers; }
    cOAL_Buffer* GetBufferByID(ALuint alID);
    virtual ALuint* GetOALBufferPointer() = 0;

    virtual void Update() = 0;
    virtual void AddBoundSource(cOAL_Source* apSource) = 0;
    virtual void RemoveBoundSource(cOAL_Source* apSource) = 0;

    int GetBuffersUsed() { return mlBuffersUsed; }

    bool IsEOF() { return mbEOF; }
    virtual bool HasBufferUnderrun() = 0;

    virtual bool NeedsRebuffering() = 0;

protected:
    eOAL_AudioDataType mType;

    tBufferVec mvBuffers;
    int mlBuffersUsed;
    long int mlSamples;
    bool mbLoop;
    bool mbEOF;
    bool mbStatus;

    double mfTotalTime;
    ALint mlFrequency;
    ALint mlChannels;
    ALenum mFormat;

    std::wstring msFilename;
};
