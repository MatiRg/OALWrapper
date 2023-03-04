/*
 * Copyright 2007-2010 (C) - Frictional Games
 *
 * This file is part of OALWrapper
 *
 * For conditions of distribution and use, see copyright notice in LICENSE
 */
/**
    @file OAL_Init.h
    @author Luis Rodero
    @date 2007-04-10
    @version 0.1
    Interface for classes handling any OpenAL Object
*/
#pragma once
#include "OAL_LoggerObject.h"

class iOAL_LowLevelObject : public iOAL_LoggerObject
{
public:
    iOAL_LowLevelObject() = default;
    virtual ~iOAL_LowLevelObject() = default;

    ////////////////////////////////////////////
    // Creation / destruction of OpenAL Objects

    virtual bool CreateLowLevelID() = 0;
    virtual bool DestroyLowLevelID() = 0;

    ////////////////////////////////////////////
    // Methods for helping in the reinitialization process (not implemented)
    virtual void SaveObjectState() = 0;
    virtual void RestoreObjectState() = 0;

    ////////////////////////////////////////////
    // Method for checking if object is valid
    virtual bool IsValidObject() = 0;

    ////////////////////////////////////////////
    // Method for returning the low level ID
    ALuint GetObjectID() { return mlObjectId; }
    ALuint* GetObjectIDPointer() { return &mlObjectId; }

    bool GetStatus() { return mbStatus; }

protected:
    ALuint mlObjectId;
    bool mbStatus;
};
