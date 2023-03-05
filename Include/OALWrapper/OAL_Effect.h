/*
 * Copyright 2007-2010 (C) - Frictional Games
 *
 * This file is part of OALWrapper
 *
 * For conditions of distribution and use, see copyright notice in LICENSE
 */
#pragma once
#include "OAL_LowLevelObject.h"

class cOAL_Effect : public iOAL_LowLevelObject
{
public:
    cOAL_Effect();
    ~cOAL_Effect();

    bool CreateLowLevelID();
    bool DestroyLowLevelID();

    void SaveObjectState() {}
    void RestoreObjectState() {}

    bool IsValidObject();

    ALuint GetEffectID() { return mlEffectId; }

    bool NeedsUpdate() { return mbNeedsUpdate; }
    void SetUpdated() { mbNeedsUpdate = false; }

    void Lock();
    void Unlock();

protected:
    ALuint mlEffectId;
    bool mbNeedsUpdate;

    SDL_mutex* mpMutex;
};
