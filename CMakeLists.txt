####### setup project and cmake #######

set(CMAKE_LEGACY_CYGWIN_WIN32 0)

# Project name
project (vorra)

# CMake version requirement
cmake_minimum_required(VERSION 3.0)

# Set flags for compiler to support c++11/c++0x
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
	message(STATUS "Found C++ 11 supported")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
	message(STATUS "Found C++ 0x supported")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
endif()

# include cmake options
include(cmake/options.cmake)

# set macro-directory
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/macros")

# set binary output location
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_subdirectory(headers)
add_subdirectory(lib)
add_subdirectory(src)
