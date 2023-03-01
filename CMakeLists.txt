cmake_minimum_required(VERSION 3.5)
project(OALWrapper CXX)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
	message(FATAL_ERROR "In-source builds are not permitted.")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	message(STATUS "Detected Clang compiler")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
	message(STATUS "Detected GCC compiler")
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	message(STATUS "Detected MSVC compiler")
else()
	message(FATAL_ERROR "Unsupported compiler.")
endif()

option(BUILD_SAMPLES "Build Samples." ON)

SET(ALL_SOURCES
    Source/OAL_AudioData.cpp
    Source/OAL_Buffer.cpp
    Source/OAL_Device.cpp
    Source/OAL_Effect.cpp
    Source/OAL_Effect_Reverb.cpp
    Source/OAL_EffectSlot.cpp
    Source/OAL_EFX.cpp
    Source/OAL_EFXManager.cpp
    Source/OAL_Filter.cpp
    Source/OAL_Helper.cpp
    Source/OAL_Init.cpp
    Source/OAL_Loaders.cpp
    Source/OAL_LoggerObject.cpp
    Source/OAL_OggSample.cpp
    Source/OAL_OggStream.cpp
    Source/OAL_WAVSample.cpp
    Source/OAL_CustomStream.cpp
    Source/OAL_Playback.cpp
    Source/OAL_Sample.cpp
    Source/OAL_Source.cpp
    Source/OAL_SourceManager.cpp
    Source/OAL_Stream.cpp
    Source/OAL_Types.cpp
)
SET(ALL_INCLUDES
    Include/OALWrapper/OAL_AudioData.h
    Include/OALWrapper/OAL_Buffer.h
    Include/OALWrapper/OAL_CustomStream.h
    Include/OALWrapper/OAL_Device.h
    Include/OALWrapper/OAL_Effect.h
    Include/OALWrapper/OAL_Effect_Reverb.h
    Include/OALWrapper/OAL_EffectSlot.h
    Include/OALWrapper/OAL_EFX.h
    Include/OALWrapper/OAL_EFXManager.h
    Include/OALWrapper/OAL_Filter.h
    Include/OALWrapper/OAL_Funcs.h
    Include/OALWrapper/OAL_Helper.h
    Include/OALWrapper/OAL_Init.h
    Include/OALWrapper/OAL_Loaders.h
    Include/OALWrapper/OAL_LoggerObject.h
    Include/OALWrapper/OAL_LowLevelObject.h
    Include/OALWrapper/OAL_OggSample.h
    Include/OALWrapper/OAL_OggStream.h
    Include/OALWrapper/OAL_Playback.h
    Include/OALWrapper/OAL_Sample.h
    Include/OALWrapper/OAL_Source.h
    Include/OALWrapper/OAL_SourceManager.h
    Include/OALWrapper/OAL_Stream.h
    Include/OALWrapper/OAL_Types.h
    Include/OALWrapper/OAL_WAVSample.h
)
set(SOURCES_FULL
	${ALL_INCLUDES}
	${ALL_SOURCES}
)

add_library(OALWrapper STATIC ${SOURCES_FULL})

set(INC_BASE Include)

if(CMAKE_SYSTEM_NAME MATCHES "Windows")
    message(STATUS "Windows system")
	if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
		set(WIN_PATH_LIBS ThirdParty/Win64-MSVC)
	elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
		set(WIN_PATH_LIBS ThirdParty/Win64-MinGW)
	endif()
    link_directories(${WIN_PATH_LIBS}/lib)
	
	set(INC_BASE ${INC_BASE} ${WIN_PATH_LIBS}/include)
	
	if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
		set(LIBS SDL2main SDL2 OpenAL32 ogg vorbis vorbisenc vorbisfile)
		set(LIBS_DBG ${LIBS})
	elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
		set(LIBS SDL2main SDL2.dll OpenAL32.dll ogg vorbis vorbisenc vorbisfile)
		set(LIBS_DBG ${LIBS})
	endif()
	
	file(GLOB WIN_DLL_LIB "${WIN_PATH_LIBS}/bin/*.dll")
    add_custom_target(copy-dll-files ALL
        COMMAND cmake -E copy ${WIN_DLL_LIB} ${CMAKE_BINARY_DIR}
        DEPENDS ${MY_TARGET})
elseif(CMAKE_SYSTEM_NAME MATCHES "Linux") # TO DO
	message(STATUS "Linux system")

	set(LIBS SDL2 openal dl)
	set(LIBS_DBG ${LIBS})
else()
	message(FATAL_ERROR "Unknown System.")
endif()

target_include_directories(OALWrapper PUBLIC ${INC_BASE})

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang|GNU")
	set(CMP_GNU -Wall -Wextra -Wunreachable-code -Wpedantic -Wshadow -Wnon-virtual-dtor -Wno-cast-function-type)
	set(CMP_OPT ${CMP_GNU})
endif()
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	set(CMP_CLN -Wall -Wextra -Wunreachable-code -Wpedantic -Wweak-vtables -Wexit-time-destructors -Wglobal-constructors -Wmissing-noreturn)
	set(CMP_OPT ${CMP_CLN})
endif()
if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	set(CMP_MSV /W4 /w44265 /w44062 /wd4201 /wd4458 /wd4996 /wd4244 /wd4061 /wd4456 /wd4457 /wd4701 /wd4267 /wd4245 /wd4100)
	set(CMP_OPT ${CMP_MSV})
endif()

target_link_libraries(OALWrapper ${LIBS})
target_compile_options(OALWrapper PRIVATE ${CMP_OPT})

if(BUILD_SAMPLES)
    add_executable(HelloSound Tests/Hello/main.cpp)
    target_include_directories(HelloSound PUBLIC ${INC_BASE})
    target_link_libraries(HelloSound ${LIBS} OALWrapper)
	target_compile_options(HelloSound PRIVATE ${CMP_OPT})
    #
    add_executable(PlaySound Tests/PlaySound/main.cpp)
    target_include_directories(PlaySound PUBLIC ${INC_BASE})
    target_link_libraries(PlaySound ${LIBS} OALWrapper)
	target_compile_options(PlaySound PRIVATE ${CMP_OPT})
endif()
