#include <OALWrapper/OAL_Funcs.h>
#include <OALWrapper/OAL_Stream.h>
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
    // Do Something ...
    //
    OAL_Close();
    return 0;
}
