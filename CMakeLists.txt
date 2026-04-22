cmake_minimum_required(VERSION 3.16)

set(PATCH_VERSION "1" CACHE STRING "Patch version")
project(cpp_dsp_showcase VERSION 0.3.${PATCH_VERSION}
                 DESCRIPTION "C++ DSP showcase with CMake, CTest and CI"
                 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

set(UPDATE_SUBMODULES OFF CACHE BOOL "Update git submodules during configure" FORCE)
include(cmake/UpdateSubmodules.cmake)

include(CTest)

add_subdirectory(external)
add_subdirectory(my_project)
