/*
 * Copyright 2007-2010 (C) - Frictional Games
 *
 * This file is part of OALWrapper
 *
 * For conditions of distribution and use, see copyright notice in LICENSE
 */
/**
    @file OAL_CustomStream.h
    @author Edward Rudd
    @date 2013-04-16
    @version 0.1
    Declaration of class for containing Custom streams
*/
#pragma once
#include "OAL_Helper.h"
#include "OAL_Stream.h"

class cOAL_CustomStream : public cOAL_Stream
{
public:
    cOAL_CustomStream(const tStreamCallbacks& aCallbacks, const tStreamInfo& aInfo, void* apData);
    ~cOAL_CustomStream();

    void Seek(float afWhere, bool abForceRebuffer);

protected:
    bool CreateFromFile(const std::wstring&) { return false; };
    bool CreateFromBuffer(const void*, size_t) { return false; };
    void Destroy();

    bool Stream(cOAL_Buffer* apDestBuffer);
    double GetTime();

    const tStreamCallbacks mCallbacks;
    const tStreamInfo mInfo;
    void* mpData;
};
