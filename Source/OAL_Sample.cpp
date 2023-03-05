/*
 * Copyright 2007-2010 (C) - Frictional Games
 *
 * This file is part of OALWrapper
 *
 * For conditions of distribution and use, see copyright notice in LICENSE
 */
/**
    @file OAL_Sample.cpp
    @author Luis Rodero
    @date 2006-10-02
    @version 0.1
    Definition of Base class for containing samples
*/
#include "OALWrapper/OAL_Sample.h"
#include "OALWrapper/OAL_Buffer.h"
#include "OALWrapper/OAL_Device.h"
#include "OALWrapper/OAL_Helper.h"
#include "OALWrapper/OAL_Source.h"
#include "OALWrapper/OAL_Types.h"

///////////////////////////////////////////////////////////
//	void Initialize ( void )
//	-	Creates a Buffer Object for the sample data
///////////////////////////////////////////////////////////

cOAL_Sample::cOAL_Sample() : iOAL_AudioData(eOAL_AudioDataType_Sample, 1)
{
}

//------------------------------------------------------------------

cOAL_Sample::~cOAL_Sample()
{
    if (mlstBoundSources.empty() == false)
    {
        tSourceListIt it;
        for (it = mlstBoundSources.begin(); it != mlstBoundSources.end(); ++it)
        {
            cOAL_Source* pSource = *it;
            pSource->Stop(false);
        }
        mlstBoundSources.clear();
    }
}

//------------------------------------------------------------------

///////////////////////////////////////////////////////////
//
//
///////////////////////////////////////////////////////////

//------------------------------------------------------------------

ALuint* cOAL_Sample::GetOALBufferPointer()
{
    return mvBuffers[0]->GetObjectIDPointer();
}
