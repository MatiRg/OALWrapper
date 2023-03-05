/*
 * Copyright 2007-2010 (C) - Frictional Games
 *
 * This file is part of OALWrapper
 *
 * For conditions of distribution and use, see copyright notice in LICENSE
 */
#include "OALWrapper/OAL_EFXManager.h"
#include "OALWrapper/OAL_Device.h"
#include "OALWrapper/OAL_Effect.h"
#include "OALWrapper/OAL_EffectSlot.h"
#include "OALWrapper/OAL_Effect_Reverb.h"
#include "OALWrapper/OAL_Filter.h"
#include <SDL2/SDL_thread.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_version.h>

int SlotUpdaterThread(void* alUnusedArg);

extern cOAL_Device* gpDevice;

// Set EFX func pointers to null (maybe this should be in OAL_Device.cpp? )

// Effect Slots

LPALGENAUXILIARYEFFECTSLOTS alGenAuxiliaryEffectSlots = nullptr;
LPALDELETEAUXILIARYEFFECTSLOTS alDeleteAuxiliaryEffectSlots = nullptr;
LPALISAUXILIARYEFFECTSLOT alIsAuxiliaryEffectSlot = nullptr;
LPALAUXILIARYEFFECTSLOTI alAuxiliaryEffectSloti = nullptr;
LPALAUXILIARYEFFECTSLOTIV alAuxiliaryEffectSlotiv = nullptr;
LPALAUXILIARYEFFECTSLOTF alAuxiliaryEffectSlotf = nullptr;
LPALAUXILIARYEFFECTSLOTFV alAuxiliaryEffectSlotfv = nullptr;
LPALGETAUXILIARYEFFECTSLOTI alGetAuxiliaryEffectSloti = nullptr;
LPALGETAUXILIARYEFFECTSLOTIV alGetAuxiliaryEffectSlotiv = nullptr;
LPALGETAUXILIARYEFFECTSLOTF alGetAuxiliaryEffectSlotf = nullptr;
LPALGETAUXILIARYEFFECTSLOTFV alGetAuxiliaryEffectSlotfv = nullptr;

// Effects

LPALGENEFFECTS alGenEffects = nullptr;
LPALDELETEEFFECTS alDeleteEffects = nullptr;
LPALISEFFECT alIsEffect = nullptr;
LPALEFFECTI alEffecti = nullptr;
LPALEFFECTIV alEffectiv = nullptr;
LPALEFFECTF alEffectf = nullptr;
LPALEFFECTFV alEffectfv = nullptr;
LPALGETEFFECTI alGetEffecti = nullptr;
LPALGETEFFECTIV alGetEffectiv = nullptr;
LPALGETEFFECTF alGetEffectf = nullptr;
LPALGETEFFECTFV alGetEffectfv = nullptr;

// Filters

LPALGENFILTERS alGenFilters = nullptr;
LPALDELETEFILTERS alDeleteFilters = nullptr;
LPALISFILTER alIsFilter = nullptr;
LPALFILTERI alFilteri = nullptr;
LPALFILTERIV alFilteriv = nullptr;
LPALFILTERF alFilterf = nullptr;
LPALFILTERFV alFilterfv = nullptr;
LPALGETFILTERI alGetFilteri = nullptr;
LPALGETFILTERIV alGetFilteriv = nullptr;
LPALGETFILTERF alGetFilterf = nullptr;
LPALGETFILTERFV alGetFilterfv = nullptr;

cOAL_EFXManager::cOAL_EFXManager() : mlNumSlots(0), mpvSlots(nullptr), mplstEffectList(nullptr), mplstFilterList(nullptr)
{
}

cOAL_EFXManager::~cOAL_EFXManager()
{
}

bool cOAL_EFXManager::Initialize(int alNumSlotsHint, int alNumSends, bool abUseThreading, int alSlotUpdateFreq)
{
    DEF_FUNC_NAME(cOAL_EFXManager::Initialize);
    FUNC_USES_AL;

    ALuint lTempSlot[256];

    // Set up every EFX function pointer

    LogMsg("", eOAL_LogVerbose_Medium, eOAL_LogMsg_Info, "Initializing EFX Manager...\n");
    LogMsg("", eOAL_LogVerbose_Medium, eOAL_LogMsg_Info, "Initializing function pointers...\n");

    // Slot funcs
    alGenAuxiliaryEffectSlots = (LPALGENAUXILIARYEFFECTSLOTS)alGetProcAddress("alGenAuxiliaryEffectSlots");
    alDeleteAuxiliaryEffectSlots = (LPALDELETEAUXILIARYEFFECTSLOTS)alGetProcAddress("alDeleteAuxiliaryEffectSlots");
    alIsAuxiliaryEffectSlot = (LPALISAUXILIARYEFFECTSLOT)alGetProcAddress("alIsAuxiliaryEffectSlot");
    alAuxiliaryEffectSloti = (LPALAUXILIARYEFFECTSLOTI)alGetProcAddress("alAuxiliaryEffectSloti");
    alAuxiliaryEffectSlotiv = (LPALAUXILIARYEFFECTSLOTIV)alGetProcAddress("alAuxiliaryEffectSlotiv");
    alAuxiliaryEffectSlotf = (LPALAUXILIARYEFFECTSLOTF)alGetProcAddress("alAuxiliaryEffectSlotf");
    alAuxiliaryEffectSlotfv = (LPALAUXILIARYEFFECTSLOTFV)alGetProcAddress("alAuxiliaryEffectSlotfv");
    alGetAuxiliaryEffectSloti = (LPALGETAUXILIARYEFFECTSLOTI)alGetProcAddress("alGetAuxiliaryEffectSloti");
    alGetAuxiliaryEffectSlotiv = (LPALGETAUXILIARYEFFECTSLOTIV)alGetProcAddress("alGetAuxiliaryEffectSlotiv");
    alGetAuxiliaryEffectSlotf = (LPALGETAUXILIARYEFFECTSLOTF)alGetProcAddress("alGetAuxiliaryEffectSlotf");
    alGetAuxiliaryEffectSlotfv = (LPALGETAUXILIARYEFFECTSLOTFV)alGetProcAddress("alGetAuxiliaryEffectSlotfv");

    // Effect funcs
    alGenEffects = (LPALGENEFFECTS)alGetProcAddress("alGenEffects");
    alDeleteEffects = (LPALDELETEEFFECTS)alGetProcAddress("alDeleteEffects");
    alIsEffect = (LPALISEFFECT)alGetProcAddress("alIsEffect");
    alEffecti = (LPALEFFECTI)alGetProcAddress("alEffecti");
    alEffectiv = (LPALEFFECTIV)alGetProcAddress("alEffectiv");
    alEffectf = (LPALEFFECTF)alGetProcAddress("alEffectf");
    alEffectfv = (LPALEFFECTFV)alGetProcAddress("alEffectfv");
    alGetEffecti = (LPALGETEFFECTI)alGetProcAddress("alGetEffecti");
    alGetEffectiv = (LPALGETEFFECTIV)alGetProcAddress("alGetEffectiv");
    alGetEffectf = (LPALGETEFFECTF)alGetProcAddress("alGetEffectf");
    alGetEffectfv = (LPALGETEFFECTFV)alGetProcAddress("alGetEffectfv");

    // Filter funcs
    alGenFilters = (LPALGENFILTERS)alGetProcAddress("alGenFilters");
    alDeleteFilters = (LPALDELETEFILTERS)alGetProcAddress("alDeleteFilters");
    alIsFilter = (LPALISFILTER)alGetProcAddress("alIsFilter");
    alFilteri = (LPALFILTERI)alGetProcAddress("alFilteri");
    alFilteriv = (LPALFILTERIV)alGetProcAddress("alFilteriv");
    alFilterf = (LPALFILTERF)alGetProcAddress("alFilterf");
    alFilterfv = (LPALFILTERFV)alGetProcAddress("alFilterfv");
    alGetFilteri = (LPALGETFILTERI)alGetProcAddress("alGetFilteri");
    alGetFilteriv = (LPALGETFILTERIV)alGetProcAddress("alGetFilteriv");
    alGetFilterf = (LPALGETFILTERF)alGetProcAddress("alGetFilterf");
    alGetFilterfv = (LPALGETFILTERFV)alGetProcAddress("alGetFilterfv");

    if (!(alGenAuxiliaryEffectSlots && alDeleteAuxiliaryEffectSlots && alIsAuxiliaryEffectSlot &&
          alAuxiliaryEffectSloti && alAuxiliaryEffectSlotiv && alAuxiliaryEffectSlotf && alAuxiliaryEffectSlotfv &&
          alGetAuxiliaryEffectSloti && alGetAuxiliaryEffectSlotiv && alGetAuxiliaryEffectSlotf && alGetAuxiliaryEffectSlotfv &&

          alGenEffects && alDeleteEffects && alIsEffect &&
          alEffecti && alEffectiv && alEffectf && alEffectfv &&
          alGetEffecti && alGetEffectiv && alGetEffectf && alGetEffectfv &&

          alGenFilters && alDeleteFilters && alIsFilter &&
          alFilteri && alFilteriv && alFilterf && alFilterfv &&
          alGetFilteri && alGetFilteriv && alGetFilterf && alGetFilterfv))
    {
        LogMsg("", eOAL_LogVerbose_Medium, eOAL_LogMsg_Error, "Failed initializing function pointers\n");
        return false;
    }
    LogMsg("", eOAL_LogVerbose_Medium, eOAL_LogMsg_Info, "Done\n");

    LogMsg("", eOAL_LogVerbose_Medium, eOAL_LogMsg_Info, "Calculating max slots...\n");

    while (mlNumSlots < alNumSlotsHint)
    {
        RUN_AL_FUNC(alGenAuxiliaryEffectSlots(1, &lTempSlot[mlNumSlots]));
        if (!AL_ERROR_OCCURED)
        {
            LogMsg("", eOAL_LogVerbose_High, eOAL_LogMsg_Info, "Effect Slot Object successfully created\n");
            ++mlNumSlots;
        }
        else
            break;
    }

    if (mlNumSlots == 0)
    {
        LogMsg("", eOAL_LogVerbose_Low, eOAL_LogMsg_Error, "Error creating Slots. Failed to initialize EFX.\n");
        return false;
    }

    LogMsg("", eOAL_LogVerbose_Medium, eOAL_LogMsg_Info, "Created %d Low Level Effect Slots, %d hinted\n", mlNumSlots, alNumSlotsHint);

    RUN_AL_FUNC(alDeleteAuxiliaryEffectSlots(mlNumSlots, lTempSlot));

    mpvSlots = new tSlotVector;
    mpvSlots->reserve(mlNumSlots);

    LogMsg("", eOAL_LogVerbose_Medium, eOAL_LogMsg_Info, "Creating High Level Effect Slots\n");

    for (int i = 0; i < mlNumSlots; i++)
    {
        cOAL_EffectSlot* pSlot = new cOAL_EffectSlot(this, i);
        mpvSlots->push_back(pSlot);
    }

    LogMsg("", eOAL_LogVerbose_Medium, eOAL_LogMsg_Info, "Done creating Effect Slots\n");

    LogMsg("", eOAL_LogVerbose_Medium, eOAL_LogMsg_Info, "Creating Filter and Effect containers\n");
    mplstEffectList = new tOALEffectList;
    mplstFilterList = new tOALFilterList;

    if (!mplstEffectList || !mplstFilterList)
    {
        LogMsg("", eOAL_LogVerbose_Low, eOAL_LogMsg_Error, "Error creating containers. Failed to initialize EFX.\n");
        return false;
    }
    else
        LogMsg("", eOAL_LogVerbose_Medium, eOAL_LogMsg_Info, "Done creating containers\n");

    mlNumSends = alNumSends;
    mbUsingThread = abUseThreading;

    // Launch updater thread
    if (mbUsingThread)
    {
        LogMsg("", eOAL_LogVerbose_Medium, eOAL_LogMsg_Info, "Launching Slot updater thread...\n");

        mlThreadWaitTime = 1000 / alSlotUpdateFreq;

        mpUpdaterThread = SDL_CreateThread(SlotUpdaterThread, "EFX Slot Updater", nullptr);
    }

    LogMsg("", eOAL_LogVerbose_Medium, eOAL_LogMsg_Info, "EFX succesfully initialized.\n");

    return true;
}

void cOAL_EFXManager::Destroy()
{
    tSlotVectorIt vSlotIterator;
    tOALEffectListIt lstEffectIterator;
    tOALFilterListIt lstFilterIterator;

    LogMsg("", eOAL_LogVerbose_Medium, eOAL_LogMsg_Command, "Destroying EFX Manager...\n");

    if (mbUsingThread)
    {
        LogMsg("", eOAL_LogVerbose_Medium, eOAL_LogMsg_Info, "Stopping Slot updater...\n");
        mbUsingThread = false;
        SDL_WaitThread(mpUpdaterThread, 0);
        mpUpdaterThread = nullptr;
    }

    LogMsg("", eOAL_LogVerbose_Medium, eOAL_LogMsg_Info, "Destroying Effect Slots...\n");

    if (mpvSlots)
    {
        for (vSlotIterator = mpvSlots->begin(); vSlotIterator != mpvSlots->end(); ++vSlotIterator)
        {
            (*vSlotIterator)->Reset();
            delete (*vSlotIterator);
        }
        mpvSlots->clear();
        delete mpvSlots;

        mpvSlots = nullptr;
    }

    LogMsg("", eOAL_LogVerbose_Medium, eOAL_LogMsg_Info, "Destroying Effects...\n");

    if (mplstEffectList)
    {
        for (lstEffectIterator = mplstEffectList->begin(); lstEffectIterator != mplstEffectList->end(); ++lstEffectIterator)
        {
            delete (*lstEffectIterator);
        }
        mplstEffectList->clear();
        delete mplstEffectList;
        mplstEffectList = nullptr;
    }

    LogMsg("", eOAL_LogVerbose_Medium, eOAL_LogMsg_Info, "Destroying Filters...\n");

    if (mplstFilterList)
    {
        for (lstFilterIterator = mplstFilterList->begin(); lstFilterIterator != mplstFilterList->end(); ++lstFilterIterator)
        {
            delete (*lstFilterIterator);
        }
        mplstFilterList->clear();
        delete mplstFilterList;
        mplstFilterList = nullptr;
    }

    LogMsg("", eOAL_LogVerbose_Medium, eOAL_LogMsg_Info, "EFX Manager successfully destroyed\n");
}

//////////////////////////////////////////////////////////////

cOAL_Filter* cOAL_EFXManager::CreateFilter()
{
    cOAL_Filter* pFilter = new cOAL_Filter;

    if (pFilter && pFilter->GetStatus())
    {
        mplstFilterList->push_back(pFilter);
        return pFilter;
    }
    else
    {
        delete pFilter;
        pFilter = nullptr;

        return nullptr;
    }
}

////////////////////////////////////////////////////////////

cOAL_Effect_Reverb* cOAL_EFXManager::CreateReverbEffect()
{
    cOAL_Effect_Reverb* pEffect = new cOAL_Effect_Reverb;

    if (pEffect && pEffect->GetStatus())
    {
        mplstEffectList->push_back(pEffect);
        return pEffect;
    }
    else
    {
        delete pEffect;
        pEffect = nullptr;

        return nullptr;
    }
}

////////////////////////////////////////////////////////////

void cOAL_EFXManager::DestroyFilter(cOAL_Filter* apFilter)
{
    if (apFilter == nullptr)
        return;

    mplstFilterList->remove(apFilter);
    delete apFilter;
}

////////////////////////////////////////////////////////////

void cOAL_EFXManager::DestroyEffect(cOAL_Effect* apEffect)
{
    if (apEffect == nullptr)
        return;

    mplstEffectList->remove(apEffect);
    delete apEffect;
}

////////////////////////////////////////////////////////////

int cOAL_EFXManager::UseEffect(cOAL_Effect* apEffect)
{
    if (apEffect == nullptr)
        return -1;

    cOAL_EffectSlot* pSlot = nullptr;
    for (int i = 0; i < mlNumSlots; ++i)
    {
        pSlot = (*mpvSlots)[i];
        if (pSlot->IsFree())
        {
            if (pSlot->AttachEffect(apEffect))
                return i;
            else
            {
                pSlot->Reset();
            }
        }
    }
    return -1;
}

void cOAL_EFXManager::UpdateSlots()
{
    cOAL_EffectSlot* pSlot;
    for (int i = 0; i < mlNumSlots; ++i)
    {
        pSlot = (*mpvSlots)[i];
        pSlot->Lock();
        pSlot->Update();
        pSlot->Unlock();
    }
}

inline int cOAL_EFXManager::GetThreadWaitTime()
{
    return mlThreadWaitTime;
}

int SlotUpdaterThread(void*)
{
    cOAL_EFXManager* pEFXManager = gpDevice->GetEFXManager();

    int lWaitTime = pEFXManager->GetThreadWaitTime();

    while (pEFXManager->IsThreadAlive())
    {
        //	While the thread lives, perform the update
        pEFXManager->UpdateSlots();
        //	And rest a bit
        SDL_Delay(lWaitTime);
    }
    return 0;
}
