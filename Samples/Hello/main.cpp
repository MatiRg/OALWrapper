#include <OALWrapper/OAL_Funcs.h>
#include <OALWrapper/OAL_Stream.h>
#include <OALWrapper/OAL_Sample.h>
#include <iostream>

class cOAL_LoggerSink: public iOAL_LoggerSink
{
public:
    void Write(eOAL_LogMsg Type, const std::string& Message) override
    {
        std::string TypeStr;
        switch(Type)
        {
        case eOAL_LogMsg_Command:
            TypeStr = "[COMMAND] ";
            break;
        case eOAL_LogMsg_Info:
            TypeStr = "[INFO] ";
            break;
        case eOAL_LogMsg_Error:
            TypeStr = "[ERROR] ";
            break;
        case eOAL_LogMsg_Text:
            [[fallthrough]];
        case eOAL_LogMsg_Default:
            [[fallthrough]];
        default:
            break;
        }
        std::cout << TypeStr << Message;
    }
};

int main(int, char*[])
{
    cOAL_LoggerSink LogSink;
    OAL_SetupLogging(true, &LogSink, eOAL_LogVerbose_High);

    cOAL_Init_Params OALInitParams;
    if( !OAL_Init(OALInitParams) )
    {
        std::cout << "Failed to Init OpenAL\n";
        return 1;
    }
    //
    cOAL_Sample* Sample1 = OAL_Sample_Load( "cell_alois.wav" );
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
    //
    return 0;
}
