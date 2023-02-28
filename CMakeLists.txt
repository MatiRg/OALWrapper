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







cmake_minimum_required (VERSION 2.8)
project (OALWrapper)

set(OALWrapper_VERSION_MAJOR 1)
set(OALWrapper_VERSION_MINOR 0)

SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMakeScripts)

SET(PRIVATE_LIB_DIR ""
    CACHE PATH "Location of local private library path to use"
)
IF(NOT APP_PRIVATE_LIB_DIR AND PRIVATE_LIB_DIR)
    SET(APP_PRIVATE_LIB_DIR ${PRIVATE_LIB_DIR})
ENDIF()

OPTION(USE_SDL2 "Use SDL2 instead of SDL1.2" OFF)
OPTION(FORCE32 "Force a 32 bit build on 64bit linux" OFF)

IF(APP_PRIVATE_LIB_DIR AND "${CMAKE_SYSTEM}" MATCHES "Linux")
    IF(NOT EXISTS ${APP_PRIVATE_LIB_DIR}/include)
        MESSAGE(FATAL_ERROR "folder named include not found in private lib dir ${APP_PRIVATE_LIB_DIR}")
    ENDIF()
    SET(EXTRA_INCLUDE_DIR ${APP_PRIVATE_LIB_DIR}/include)
    IF(NOT PRIVATE_LIB_SUFFIX)
        IF(CMAKE_SIZEOF_VOID_P MATCHES "8" AND NOT(FORCE32))
            if(EXISTS ${APP_PRIVATE_LIB_DIR}/linux64)
                SET(PRIVATE_LIB_SUFFIX "linux64")
            else(EXISTS ${APP_PRIVATE_LIB_DIR}/lib64)
                SET(PRIVATE_LIB_SUFFIX "lib64")
            endif()
        ELSE()
            if(EXISTS ${APP_PRIVATE_LIB_DIR}/linux32)
                SET(PRIVATE_LIB_SUFFIX "linux32")
            elseif(EXISTS ${APP_PRIVATE_LIB_DIR}/linux)
                SET(PRIVATE_LIB_SUFFIX "linux")
            else(EXISTS ${APP_PRIVATE_LIB_DIR}/lib)
                SET(PRIVATE_LIB_SUFFIX "lib")
            endif()
        ENDIF()
    ENDIF()
    IF(NOT PRIVATE_LIB_SUFFIX)
        MESSAGE(FATAL_ERROR "could not find lib directory in private lib dir ${APP_PRIVATE_LIB_DIR}")
    ENDIF()

    FIND_LIBRARY(OGG_LIBRARY ogg PATHS ${APP_PRIVATE_LIB_DIR}/${PRIVATE_LIB_SUFFIX} PATH_SUFFIXES lib NO_DEFAULT_PATH)
    FIND_LIBRARY(VORBIS_LIBRARY vorbis PATHS ${APP_PRIVATE_LIB_DIR}/${PRIVATE_LIB_SUFFIX} PATH_SUFFIXES lib NO_DEFAULT_PATH)
    FIND_LIBRARY(VORBISFILE_LIBRARY vorbisfile PATHS ${APP_PRIVATE_LIB_DIR}/${PRIVATE_LIB_SUFFIX} PATH_SUFFIXES lib NO_DEFAULT_PATH)

    FIND_LIBRARY(OPENAL_LIBRARY openal PATHS ${APP_PRIVATE_LIB_DIR}/${PRIVATE_LIB_SUFFIX} PATH_SUFFIXES lib NO_DEFAULT_PATH)

    IF(USE_SDL2)
        FIND_LIBRARY(SDL_LIBRARY SDL2 PATHS ${APP_PRIVATE_LIB_DIR}/${PRIVATE_LIB_SUFFIX} PATH_SUFFIXES lib NO_DEFAULT_PATH)
        FIND_PATH(SDL_INCLUDE_DIR SDL.h
            PATHS ${APP_PRIVATE_LIB_DIR}/include
            PATH_SUFFIXES SDL2
            NO_DEFAULT_PATH
        )
    ELSE()
        FIND_LIBRARY(SDL_LIBRARY SDL PATHS ${APP_PRIVATE_LIB_DIR}/${PRIVATE_LIB_SUFFIX} PATH_SUFFIXES lib NO_DEFAULT_PATH)
        FIND_PATH(SDL_INCLUDE_DIR SDL.h
            PATHS ${APP_PRIVATE_LIB_DIR}/include
            PATH_SUFFIXES SDL
            NO_DEFAULT_PATH
        )
    ENDIF()
ELSEIF(APP_PRIVATE_LIB_DIR AND EMSCRIPTEN)
    SET(PRIVATE_LIB_SUFFIX emscripten)
    SET(EXTRA_INCLUDE_DIR ${APP_PRIVATE_LIB_DIR}/include)
    FIND_LIBRARY(OGG_LIBRARY ogg PATHS ${APP_PRIVATE_LIB_DIR}/${PRIVATE_LIB_SUFFIX} NO_DEFAULT_PATH NO_CMAKE_FIND_ROOT_PATH)
    FIND_LIBRARY(VORBIS_LIBRARY vorbis PATHS ${APP_PRIVATE_LIB_DIR}/${PRIVATE_LIB_SUFFIX} NO_DEFAULT_PATH NO_CMAKE_FIND_ROOT_PATH)
    FIND_LIBRARY(VORBISFILE_LIBRARY vorbisfile PATHS ${APP_PRIVATE_LIB_DIR}/${PRIVATE_LIB_SUFFIX} NO_DEFAULT_PATH NO_CMAKE_FIND_ROOT_PATH)

    IF(USE_SDL2)
        FIND_LIBRARY(SDL_LIBRARY SDL2 PATHS ${APP_PRIVATE_LIB_DIR}/${PRIVATE_LIB_SUFFIX} NO_DEFAULT_PATH NO_CMAKE_FIND_ROOT_PATH)
    ELSE()
        MESSAGE(FATAL_ERROR "SDL1.2 not supported on Emscripten")
    ENDIF()

    FIND_PATH(SDL_INCLUDE_DIR SDL.h
        PATHS ${APP_PRIVATE_LIB_DIR}/include
        PATH_SUFFIXES SDL2
        NO_DEFAULT_PATH
        NO_CMAKE_FIND_ROOT_PATH
    )

    #  We don't need to find an OpenAL library as it's built-in to emscripten
    unset(OPENAL_LIBRARY)
    unset(OPENAL_INCLUDE_DIR)
ELSEIF(APP_PRIVATE_LIB_DIR AND APPLE)
    if(IOS)
        SET(PRIVATE_LIB_SUFFIX ios)
        SET(EXTRA_INCLUDE_DIR ${APP_PRIVATE_LIB_DIR}/include)
        FIND_LIBRARY(OGG_LIBRARY ogg PATHS ${APP_PRIVATE_LIB_DIR}/${PRIVATE_LIB_SUFFIX} NO_DEFAULT_PATH)
        FIND_LIBRARY(VORBIS_LIBRARY vorbis PATHS ${APP_PRIVATE_LIB_DIR}/${PRIVATE_LIB_SUFFIX} NO_DEFAULT_PATH)
        FIND_LIBRARY(VORBISFILE_LIBRARY vorbisfile PATHS ${APP_PRIVATE_LIB_DIR}/${PRIVATE_LIB_SUFFIX} NO_DEFAULT_PATH)
    else()
        IF(NOT PRIVATE_LIB_SUFFIX)
            SET(PRIVATE_LIB_SUFFIX macosx)
        ENDIF()
        SET(EXTRA_INCLUDE_DIR ${APP_PRIVATE_LIB_DIR}/include)
        FIND_LIBRARY(OGG_LIBRARY Ogg PATHS ${APP_PRIVATE_LIB_DIR}/${PRIVATE_LIB_SUFFIX} NO_DEFAULT_PATH)
        FIND_LIBRARY(VORBIS_LIBRARY Vorbis PATHS ${APP_PRIVATE_LIB_DIR}/${PRIVATE_LIB_SUFFIX} NO_DEFAULT_PATH)
        if (VORBIS_LIBRARY AND NOT VORBIS_LIBRARY MATCHES /.framework$/)
            FIND_LIBRARY(VORBISFILE_LIBRARY vorbisfile PATHS ${APP_PRIVATE_LIB_DIR}/${PRIVATE_LIB_SUFFIX} NO_DEFAULT_PATH)
        endif()
    endif()

    IF(USE_SDL2)
        FIND_LIBRARY(SDL_LIBRARY SDL2 PATHS ${APP_PRIVATE_LIB_DIR}/${PRIVATE_LIB_SUFFIX} NO_DEFAULT_PATH)
    ELSE()
        FIND_LIBRARY(SDL_LIBRARY SDL PATHS ${APP_PRIVATE_LIB_DIR}/${PRIVATE_LIB_SUFFIX} NO_DEFAULT_PATH)
    ENDIF()

    FIND_PATH(SDL_INCLUDE_DIR SDL.h
        PATHS ${APP_PRIVATE_LIB_DIR}/include
        PATH_SUFFIXES SDL2
        NO_DEFAULT_PATH
    )

    FIND_PACKAGE(OpenAL)
ELSE()
    FIND_PACKAGE(OGG)
    FIND_PACKAGE(Vorbis)
    IF(NOT(APPLE))
        FIND_PACKAGE(VorbisFile)
    ENDIF()
    FIND_PACKAGE(OpenAL)
    IF(USE_SDL2)
        FIND_PACKAGE(SDL2)
    ELSE()
        FIND_PACKAGE(SDL)
    ENDIF()

    LIST(APPEND OALWRAPPER_INCLUDE_PATHS
        PRIVATE ${OGG_INCLUDE_DIR}
        PRIVATE ${VORBIS_INCLUDE_DIR}
        PUBLIC ${OPENAL_INCLUDE_DIR}
    )
    if(VORBISFILE_INCLUDE_DIR)
        LIST(APPEND OALWRAPPER_INCLUDE_PATHS
            PRIVATE ${VORBISFILE_INCLUDE_DIR}
        )
    endif()
ENDIF()

LIST(APPEND OALWRAPPER_INCLUDE_PATHS
    PUBLIC include
    PRIVATE ${SDL_INCLUDE_DIR}
)
if(EXTRA_INCLUDE_DIR)
    LIST(APPEND OALWRAPPER_INCLUDE_PATHS
        PRIVATE ${EXTRA_INCLUDE_DIR}
    )
endif()
SET(all_sources
    sources/OAL_AudioData.cpp
    sources/OAL_Buffer.cpp
    sources/OAL_Device.cpp
    sources/OAL_Effect.cpp
    sources/OAL_Effect_Reverb.cpp
    sources/OAL_EffectSlot.cpp
    sources/OAL_EFX.cpp
    sources/OAL_EFXManager.cpp
    sources/OAL_Filter.cpp
    sources/OAL_Helper.cpp
    sources/OAL_Init.cpp
    sources/OAL_Loaders.cpp
    sources/OAL_LoggerObject.cpp
    sources/OAL_OggSample.cpp
    sources/OAL_OggStream.cpp
    sources/OAL_WAVSample.cpp
    sources/OAL_CustomStream.cpp
    sources/OAL_Playback.cpp
    sources/OAL_Sample.cpp
    sources/OAL_Source.cpp
    sources/OAL_SourceManager.cpp
    sources/OAL_Stream.cpp
    sources/OAL_Types.cpp
)

SET(all_includes
    include//OALWrapper/OAL_AudioData.h
    include//OALWrapper/OAL_Buffer.h
    include//OALWrapper/OAL_CustomStream.h
    include//OALWrapper/OAL_Device.h
    include//OALWrapper/OAL_Effect.h
    include//OALWrapper/OAL_Effect_Reverb.h
    include//OALWrapper/OAL_EffectSlot.h
    include//OALWrapper/OAL_EFX.h
    include//OALWrapper/OAL_EFXManager.h
    include//OALWrapper/OAL_Filter.h
    include//OALWrapper/OAL_Funcs.h
    include//OALWrapper/OAL_Helper.h
    include//OALWrapper/OAL_Init.h
    include//OALWrapper/OAL_Loaders.h
    include//OALWrapper/OAL_LoggerObject.h
    include//OALWrapper/OAL_LowLevelObject.h
    include//OALWrapper/OAL_OggSample.h
    include//OALWrapper/OAL_OggStream.h
    include//OALWrapper/OAL_Playback.h
    include//OALWrapper/OAL_Sample.h
    include//OALWrapper/OAL_Source.h
    include//OALWrapper/OAL_SourceManager.h
    include//OALWrapper/OAL_Stream.h
    include//OALWrapper/OAL_Types.h
    include//OALWrapper/OAL_WAVSample.h
)

IF(USE_SDL2)
    add_definitions(-DUSE_SDL2)
ENDIF()

add_library(OALWrapper STATIC
    ${all_sources}
    ${all_includes}
)
TARGET_LINK_LIBRARIES(OALWrapper
    ${VORBISFILE_LIBRARY}
    ${VORBIS_LIBRARY}
    ${OGG_LIBRARY}
    ${OPENAL_LIBRARY}
    ${SDL_LIBRARY}
)
TARGET_INCLUDE_DIRECTORIES(OALWrapper
    ${OALWRAPPER_INCLUDE_PATHS}
)

ADD_EXECUTABLE(SimpleTest EXCLUDE_FROM_ALL tests/Simple/main.cpp)
TARGET_LINK_LIBRARIES(SimpleTest OALWrapper)

ADD_EXECUTABLE(PlaySound EXCLUDE_FROM_ALL tests/PlaySound/main.cpp)
TARGET_LINK_LIBRARIES(PlaySound OALWrapper)

ADD_EXECUTABLE(Playlist EXCLUDE_FROM_ALL tests/Playlist/main.cpp)
TARGET_LINK_LIBRARIES(Playlist OALWrapper)

ADD_EXECUTABLE(CustomStream EXCLUDE_FROM_ALL tests/CustomStream/main.cpp)
TARGET_LINK_LIBRARIES(CustomStream OALWrapper)

# Exports
#SET(OALWRAPPER_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include PARENT_SCOPE)
