/*
 * Copyright 2007-2010 (C) - Frictional Games
 *
 * This file is part of OALWrapper
 *
 * For conditions of distribution and use, see copyright notice in LICENSE
 */
#pragma once
#include "OAL_LowLevelObject.h"
#include "OAL_Types.h"

class cOAL_Filter : public iOAL_LowLevelObject
{
public:
    cOAL_Filter();
    ~cOAL_Filter();

    bool CreateLowLevelID();
    bool DestroyLowLevelID();

    void SaveObjectState() {}
    void RestoreObjectState() {}

    bool IsValidObject();

    void SetType(eOALFilterType aeType);
    void SetGain(float afGain);
    void SetGainHF(float afGainHF);
    void SetGainLF(float afGainHF);

    eOALFilterType GetType() { return mType; }
    float GetGain() { return mfGain; }
    float GetGainHF() { return mfGainHF; }
    float GetGainLF() { return mfGainLF; }

protected:
    float mfGain;
    float mfGainHF;
    float mfGainLF;
    eOALFilterType mType;
};
