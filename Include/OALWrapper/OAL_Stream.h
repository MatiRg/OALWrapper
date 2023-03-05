/*
 * Copyright 2007-2010 (C) - Frictional Games
 *
 * This file is part of OALWrapper
 *
 * For conditions of distribution and use, see copyright notice in LICENSE
 */
#pragma once
#include "OAL_AudioData.h"

class cOAL_Stream : public iOAL_AudioData
{
public:
    cOAL_Stream();
    virtual ~cOAL_Stream();

    /////////////////////////////////////

    void AddBoundSource(cOAL_Source* apSource)
    {
        if (apSource)
            mpBoundSource = apSource;
    }
    void RemoveBoundSource(cOAL_Source* apSource)
    {
        if (apSource && (apSource == mpBoundSource))
            mpBoundSource = nullptr;
    }

    void Update();

    void DoBuffering();

    virtual void Seek(float afWhere, bool abForceRebuffer = true) = 0;
    virtual double GetTime() = 0;

    inline bool IsLocked() { return mpBoundSource != nullptr; }

    // Streaming Config stuff
    static void SetBufferCount(unsigned int alBufferCount)
    {
        if (alBufferCount >= 1)
            mlBufferCount = alBufferCount;
    }
    static void SetBufferSize(unsigned int alBufferSize)
    {
        if (alBufferSize >= STREAMING_BLOCK_SIZE)
            mlBufferSize = alBufferSize;
    }

    static unsigned int GetBufferSize() { return mlBufferSize; }
    static unsigned int GetBufferCount() { return mlBufferCount; }

    ALuint* GetOALBufferPointer() { return mvOALBufferIDs; }

    double GetProcessedBuffersTime() { return mfProcessedBuffersTime; }

    bool HasBufferUnderrun();
    bool NeedsRebuffering() { return mbNeedsRebuffering; }

protected:
    virtual bool Stream(cOAL_Buffer* apDestBuffer) = 0;

    cOAL_Source* mpBoundSource;

    static unsigned int mlBufferCount;
    static unsigned int mlBufferSize;

    bool mbNeedsRebuffering;

    double mfProcessedBuffersTime;

    char* mpPCMBuffer;
    ALuint* mvOALBufferIDs;
};
