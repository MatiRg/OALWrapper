/*
 * Copyright 2007-2010 (C) - Frictional Games
 *
 * This file is part of OALWrapper
 *
 * For conditions of distribution and use, see copyright notice in LICENSE
 */
#pragma once
#include "OAL_Helper.h"
#include "OAL_LoggerObject.h"
#include "OAL_Types.h"

class cOAL_SourceManager : public iOAL_LoggerObject
{
public:
    cOAL_SourceManager();
    ~cOAL_SourceManager();

    cOAL_Source* GetSource(int alSourceHandle,
                           bool abSkipRefCountCheck = false);
    cOAL_Source* GetAvailableSource(unsigned int alPriority, int alNumOfVoices);

    bool Initialize(bool abManageVoices,
                    int alNumSourcesHint,
                    bool abUseThreading,
                    int alUpdateFreq,
                    int alEFXSends = 0);
    void Destroy();

    void AddActiveStream(cOAL_Source* apSource);
    void UpdateStreaming();

    void ReserveVoices(int alNum)
    {
        if (!mbManageVoices)
            alNum = 1;
        mlAvailableVoices -= alNum;
    }

    void ReleaseVoices(int alNum)
    {
        if (!mbManageVoices)
            alNum = 1;
        mlAvailableVoices += alNum;
    }

    bool IsThreadAlive() { return mbUseThreading; }
    int GetThreadWaitTime() { return mlThreadWaitTime; }

    int GetNumVoices() { return mlNumOfVoices; }

	const tSourceVec& GetSources() const { return mvSources; }
private:
    void LockStreamList();
    void UnlockStreamList();

    int GetUnpackedSourceId(int alHandle);
    int GetUnpackedRefCount(int alHandle);

    bool mbManageVoices;

    int mlNumOfVoices;
    int mlAvailableVoices;

    SDL_mutex* mpStreamListMutex;
    SDL_Thread* mpUpdaterThread;
    int mlThreadWaitTime;
    bool mbUseThreading;

    tSourceVec mvSources;
    tSourceList mlstStreamingSources;
};
