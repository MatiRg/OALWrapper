/*
 * Copyright 2007-2010 (C) - Frictional Games
 *
 * This file is part of OALWrapper
 *
 * For conditions of distribution and use, see copyright notice in LICENSE
 */
/**
    @file OAL_Source.h
    @author Luis Rodero
    @date 2006-10-10
    @version 0.1
    Wrapping for OpenAL Source Objects
*/
#pragma once
#include "OAL_AudioData.h"
#include "OAL_Effect.h"
#include "OAL_EffectSlot.h"
#include "OAL_Filter.h"
#include "OAL_LowLevelObject.h"
#include "OAL_SourceManager.h"
#include "OAL_Types.h"
#include <limits>

static constexpr int OAL_SEND_DIRECT = 0x10000000;
static constexpr int OAL_SEND_AUX0 = 1;
static constexpr int OAL_SEND_AUX1 = 2;
static constexpr int OAL_SEND_AUX2 = 4;
static constexpr int OAL_SEND_AUX3 = 8;

enum eOAL_SourceParam
{
    eOAL_SourceParam_Gain,
    eOAL_SourceParam_Pitch,
    eOAL_SourceParam_Position,
    eOAL_SourceParam_PositionRelative,
    eOAL_SourceParam_Velocity,
    eOAL_SourceParam_Buffer,
    eOAL_SourceParam_Default
};

class cOAL_SourceManager;

class cOAL_SourceSend
{
public:
    cOAL_SourceSend() : mpSlot(nullptr),
                        mpFilter(nullptr)
    {
    }
    ~cOAL_SourceSend() = default;

    ALuint GetSlot()
    {
        if (mpSlot)
            return mpSlot->GetObjectID();
        return AL_EFFECTSLOT_NULL;
    }
    ALuint GetFilter()
    {
        if (mpFilter)
            return mpFilter->GetObjectID();
        return AL_FILTER_NULL;
    }

    void SetSlot(cOAL_EffectSlot* apSlot) { mpSlot = apSlot; }
    void SetFilter(cOAL_Filter* apFilter) { mpFilter = apFilter; }
private:
    cOAL_EffectSlot* mpSlot;
    cOAL_Filter* mpFilter;
};

class cOAL_Source : public iOAL_LowLevelObject
{
public:
    cOAL_Source(cOAL_SourceManager* apSourceManager, int alId, int alSends);
    ~cOAL_Source();

    //////////////////////////////////////
    // iOAL_LowLevelObject impl.

    bool CreateLowLevelID();
    bool DestroyLowLevelID();

    void SaveObjectState() {}
    void RestoreObjectState() {}

    bool IsValidObject();

    //////////////////////////////////////
    // Threading stuff
    void Lock();
    void Unlock();

    /////////////////////////////////////
    // Playback management
    int BindData(cOAL_Sample* apSample);
    int BindData(cOAL_Stream* apStream);
    void Play();
    void Stop(bool abRemove = true);
    void Pause(bool abPaused);

    ////////////////////////////////////
    // Attribute modifiers
    
    void SetGain(float afGain);
    void SetPitch(float afPitch);
    void SetPosition(const float* apPos);
    void SetPositionRelative(bool abRelative);
    void SetVelocity(const float* apVel);
    void SetLoop(bool abLoop);

    void SetMinDistance(const float afMin);
    void SetMaxDistance(const float afMax);
    void SetAttenuation(const float afAttenuation);

    void SetElapsedTime(double afTime);
    void SetPriority(const ALuint alX);

    float GetGain() { return mfGain; }
    float GetPitch() { return mfPitch; }

    ///////////////////////////////////
    // Methods for retrieving information
    int GetId() { return mlId; }
    ALuint GetPriority() { return mlPriority; }
    eOAL_SourceStatus GetSourceStatus();
    eOAL_AudioDataType GetSourceType();
    int GetProcessedBuffers();
    int GetQueuedBuffers();
    double GetElapsedTime();
    double GetTotalTime();

    unsigned int GetRefCount() { return mlRefCount; }

    // EFX related methods

    void SetDirectFilter(cOAL_Filter* apFilter);
    void SetAuxSend(int alSendId, cOAL_EffectSlot* apSlot, cOAL_Filter* apFilter);
    void SetAuxSendSlot(int alSendId, cOAL_EffectSlot* apSlot);
    void SetAuxSendFilter(int alSendId, cOAL_Filter* apFilter);

    // Built in filter methods
    void SetFilterType(eOALFilterType aeType);
    void SetFilterGain(float afGain);
    void SetFilterGainHF(float afGainHF);
    void SetFilterGainLF(float afGainLF);
    void SetFilterEnabled(bool abEnabled, int alFlags);

    void UpdateFiltering(unsigned int alSends);
    void UpdateFiltering();

    // Misc methods
    void Update();

    // Special
    void IncRefCount();
private:
    friend class cOAL_Stream;

    int mlId;
    unsigned int mlPriority;
    unsigned int mlRefCount;

    SDL_mutex* mpSourceMutex;

    cOAL_SourceManager* mpSourceManager;

    iOAL_AudioData* mpAudioData;

    bool mbLoop;
    bool mbPlaying;
    bool mbPaused;

    bool mbNeedsReset;

    float mfGain;
    float mfPitch;
    float mvPos[3];
    float mvVel[3];
    bool mbRelativePosition;

    float mfMinDistance = 1.0f;
    float mfMaxDistance = std::numeric_limits<float>::max();
    float mfAttenuation = 1.0f;

    std::vector<cOAL_SourceSend*> mvSends;

    cOAL_Filter* mpFilter;
    cOAL_Filter* mpDirectFilter;

    void Queue(cOAL_Buffer* apBuffer);
    cOAL_Buffer* Unqueue();

    void LowLevelPlay();
    void LowLevelPause();
    void LowLevelStop();
    bool LowLevelSet(eOAL_SourceParam aeParam);
    void LowLevelSetMuted(bool abX);

    int GetPackedHandle(int alRef, int alId);
};
