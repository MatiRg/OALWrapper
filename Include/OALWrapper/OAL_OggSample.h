/*
 * Copyright 2007-2010 (C) - Frictional Games
 *
 * This file is part of OALWrapper
 *
 * For conditions of distribution and use, see copyright notice in LICENSE
 */
/**
    @file OAL_OggSample.h
    @author Luis Rodero
    @date 2006-10-02
    @version 0.1
    Derived class for containing Ogg Vorbis Sample data
*/
#pragma once
#include "OAL_Sample.h"

struct OggVorbis_File;

class cOAL_OggSample : public cOAL_Sample
{
public:
    bool CreateFromFile(const std::wstring& asFilename);
    bool CreateFromBuffer(const void* apBuffer, size_t aSize);
private:
    bool LoadOgg(OggVorbis_File& ovFileHandle);
};
