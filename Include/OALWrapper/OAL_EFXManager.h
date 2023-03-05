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

enum OAL_Effect_Type
{
    OAL_Effect_Type_Reverb,
    OAL_Effect_Type_Default
};

enum OAL_Filter_Type
{
    OAL_Filter_Type_LowPass,
    OAL_Filter_Type_HighPass,
    OAL_Filter_Type_BandPass,
    OAL_Filter_Type_Default
};

class cOAL_EFXManager : public iOAL_LoggerObject
{
public:
    cOAL_EFXManager();
    ~cOAL_EFXManager();

    cOAL_EffectSlot* GetEffectSlot(int alSlotHandle)
    {
        if ((alSlotHandle >= 0) && ((unsigned int)alSlotHandle < mpvSlots->size()))
            return (*mpvSlots)[alSlotHandle];
        else
            return nullptr;
    }

    bool Initialize(int alNumSlotsHint, int alNumSends, bool abUseThread, int alSlotUpdateFreq);
    void Destroy();

    cOAL_Effect_Reverb* CreateReverbEffect();

    cOAL_Filter* CreateFilter();

    void DestroyEffect(cOAL_Effect* apEffect);
    void DestroyFilter(cOAL_Filter* apFilter);

    int AssignSlot(cOAL_Effect* apEffect);

    int GetNumSlots() { return mlNumSlots; }
    int GetNumSends() { return mlNumSends; }

    int UseEffect(cOAL_Effect* apEffect);

    void UpdateSlots();

    bool IsThreadAlive() { return mbUsingThread; }
    int GetThreadWaitTime();

    std::string GetIDString();

private:
    int mlNumSlots;
    int mlNumSends;

    tSlotVector* mpvSlots;
    tOALEffectList* mplstEffectList;
    tOALFilterList* mplstFilterList;

    bool mbUsingThread;
    SDL_Thread* mpUpdaterThread;
    int mlThreadWaitTime;
};
