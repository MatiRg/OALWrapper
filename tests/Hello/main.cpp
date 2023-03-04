#include <OALWrapper/OAL_Funcs.h>
#include <OALWrapper/OAL_Stream.h>
#include <OALWrapper/OAL_Sample.h>
#include <iostream>

int main(int, char*[])
{
    OAL_SetupLogging(true, eOAL_LogOutput_File, eOAL_LogVerbose_High);
    cOAL_Init_Params OALInitParams;
    if( !OAL_Init(OALInitParams) )
    {
        std::cout << "Failed to Init OpenAL\n";
        return 1;
    }
    //
    cOAL_Sample* Sample1 = OAL_Sample_Load( "06_amb.ogg" );
    cOAL_Stream* Stream1 = OAL_Stream_Load( "06_amb.ogg" );
    if( Sample1 && Stream1 )
    {
        // Play 
        const int s1 = OAL_Sample_Play(OAL_FREE, Sample1, 1.0f, false, 10);
        while( OAL_Source_IsPlaying(s1) ) {}
        OAL_Source_Stop(s1);
        //
        std::cout << "Playing Stream\n";
        // Play Stream
        const int s2 = OAL_Stream_Play(OAL_FREE, Stream1, 1.0f, false);
        while( OAL_Source_IsPlaying(s2) ) {}
        OAL_Source_Stop(s2);
        //
    }
    OAL_Sample_Unload(Sample1);
    OAL_Stream_Unload(Stream1);

    //
    OAL_Close();
    return 0;
}
