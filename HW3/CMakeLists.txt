cmake_minimum_required(VERSION 3.3.0)
project(hw3)


if(CMAKE_CONFIGURATION_TYPES)
    message("Multi-configuration generator")
    set(CMAKE_CONFIGURATION_TYPES "Debug;MinSizeRel" CACHE STRING "MSVC Configuration Types" FORCE)
else()
    message("Single-configuration generator")
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(PREDEFINED_TARGETS_FOLDER "zzzCustomTargets")

file(GLOB SOURCES "*.cpp")

if( NOT TargOS )
   set( TargOS "osLINUX" )
endif()

message("Target OS is TargOS: ${TargOS}")

#Select which OS you would like to target for compilation
#set( TargOS "osLINUX" )
#set( TargOS "osWINDOWS" )

if( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
    "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" )
   #This is where GNU/Clang specific compiler flags specifically for building the engine are located.
   SET( warnings "${warnings} -Wall -Wextra" )
   SET( cppFlags "-std=c++17 -Wfatal-errors -D${TargOS}" ) #GNU/Clang specific CPP compiler flags
elseif( "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC" )
   #This is where MSVC specific compiler flags specifically for building the engine are located.
   SET( warnings "${warnings} /WX /MP" ) #treat all compiler warnings as errors
   SET( cppFlags "/std:c++17 /D${TargOS}" ) #GNU/Clang specific CPP compiler flags
endif()

SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${warnings} ${cppFlags}" )
SET( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${warnings}" )
MESSAGE( STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}" )
MESSAGE( STATUS "CMAKE_C_FLAGS  : ${CMAKE_C_FLAGS}" )


add_executable(hw3 ${SOURCES})
