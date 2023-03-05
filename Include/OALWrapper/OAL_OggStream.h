/*
 * Copyright 2007-2010 (C) - Frictional Games
 *
 * This file is part of OALWrapper
 *
 * For conditions of distribution and use, see copyright notice in LICENSE
 */
/**
    @file OAL_OggStream.h
    @author Luis Rodero
    @date 2006-10-06
    @version 0.1
    Declaration of class for containing Ogg Vorbis streams
*/
#pragma once
#include "OAL_Helper.h"
#include "OAL_Stream.h"

struct stb_vorbis;

class cOAL_OggStream : public cOAL_Stream
{
public:
    cOAL_OggStream();
    ~cOAL_OggStream();

    void Seek(float afWhere, bool abForceRebuffer);

protected:
    bool CreateFromFile(const std::wstring& asFileName);
    bool CreateFromBuffer(const void* apBuffer, size_t aSize);
    void Destroy();

    bool Stream(cOAL_Buffer* apDestBuffer);
    double GetTime();

    bool CreateInternal();
protected:
    stb_vorbis* STBFile = nullptr;
    float mfActualTime = 0.0f;
};
