/*
 * Copyright 2007-2010 (C) - Frictional Games
 *
 * This file is part of OALWrapper
 *
 * For conditions of distribution and use, see copyright notice in LICENSE
 */
#pragma once
#include "OAL_AudioData.h"
#include "OAL_LowLevelObject.h"
#include "OAL_Types.h"

class iOAL_AudioData;

class cOAL_Buffer : public iOAL_LowLevelObject
{
public:
    cOAL_Buffer(iOAL_AudioData* apParent);
    ~cOAL_Buffer();

    ////////////////////////////////////
    // iOAL_LowLevelObject impl
    bool CreateLowLevelID();
    bool DestroyLowLevelID();

    void SaveObjectState() {}
    void RestoreObjectState() {}

    bool IsValidObject();

    bool Feed(ALvoid* apPCMData, int alDataSize, double afStartTime = 0.0);
    double GetStartTime() { return mfStartTime; }
    double GetBufferTime() { return mfBufferTime; }

protected:
    iOAL_AudioData* mpParent;
    double mfStartTime;
    double mfBufferTime;
};
