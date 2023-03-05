/*
 * Copyright 2007-2010 (C) - Frictional Games
 *
 * This file is part of OALWrapper
 *
 * For conditions of distribution and use, see copyright notice in LICENSE
 */
#include "OALWrapper/OAL_Stream.h"
#include "OALWrapper/OAL_Buffer.h"
#include "OALWrapper/OAL_Device.h"
#include "OALWrapper/OAL_Helper.h"
#include "OALWrapper/OAL_Source.h"
#include "OALWrapper/OAL_Types.h"
#include <cstdlib>

unsigned int cOAL_Stream::mlBufferSize = STREAMING_BLOCK_SIZE;
unsigned int cOAL_Stream::mlBufferCount = 4u;

cOAL_Stream::cOAL_Stream() : iOAL_AudioData(eOAL_AudioDataType_Stream, mlBufferCount), mpBoundSource(nullptr)
{
    mpPCMBuffer = (char*)malloc(mlBufferSize * sizeof(char));
    mvOALBufferIDs = (ALuint*)malloc(mlBufferCount * sizeof(ALuint));
    for (int i = 0; i < (int)mlBufferCount; ++i)
        mvOALBufferIDs[i] = mvBuffers[i]->GetObjectID();

    mfProcessedBuffersTime = 0;
}

cOAL_Stream::~cOAL_Stream()
{
    if (mpBoundSource != nullptr)
        mpBoundSource->Stop();

    free(mpPCMBuffer);
    free(mvOALBufferIDs);
}

//-----------------------------------------------------------------------------------

void cOAL_Stream::Update()
{
    if (mpBoundSource == nullptr)
        return;

    if (mbNeedsRebuffering)
    {
        bool bPlaying = mpBoundSource->mbPlaying;
        bool bPaused = mpBoundSource->mbPaused;
        mpBoundSource->LowLevelStop();
        mlBuffersUsed = 0;
        mfProcessedBuffersTime = GetTime();
        int lQueuedBuffers = mpBoundSource->GetQueuedBuffers();
        for (int i = 0; i < lQueuedBuffers; ++i)
            mpBoundSource->Unqueue();

        for (int i = 0; i < (int)mvBuffers.size(); ++i)
        {
            cOAL_Buffer* pBuffer = mvBuffers[i];
            if (Stream(pBuffer))
            {
                ++mlBuffersUsed;
                mpBoundSource->Queue(pBuffer);
            }
        }
        if (mlBuffersUsed != 0)
            mbNeedsRebuffering = false;
        if (bPlaying && bPaused == false)
            mpBoundSource->LowLevelPlay();
    }

    if (mlBuffersUsed == 0)
        return;

    int lProcessedBuffers = mpBoundSource->GetProcessedBuffers();
    // For every buffer that has been played, unqueue it, refill it with streamed data and enqueue it again
    while (lProcessedBuffers)
    {
        // Then perform the actual streaming
        cOAL_Buffer* pBuffer = mpBoundSource->Unqueue();
        mfProcessedBuffersTime += pBuffer->GetBufferTime();
        if (mfProcessedBuffersTime > mfTotalTime)
            mfProcessedBuffersTime -= mfTotalTime;

        if (Stream(pBuffer))
        {
            mpBoundSource->Queue(pBuffer);
        }
        else
        {
            if (mbEOF && (mbLoop || mpBoundSource->mbLoop))
            {
                Seek(0, false);
                Stream(pBuffer);
                mpBoundSource->Queue(pBuffer);
            }
        }
        lProcessedBuffers--;
    }
}

//-----------------------------------------------------------------------------------

void cOAL_Stream::DoBuffering()
{
}

//-----------------------------------------------------------------------------------

bool cOAL_Stream::HasBufferUnderrun()
{
    if (mpBoundSource == nullptr)
        return false;

    if (mbEOF == false)
    {
        return true;
    }

    if (mpBoundSource->GetQueuedBuffers() != mpBoundSource->GetProcessedBuffers())
    {
        return true;
    }

    return false;
}
