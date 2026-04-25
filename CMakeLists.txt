cmake_minimum_required(VERSION 3.16)

set(PATCH_VERSION "1" CACHE STRING "Patch version")
project(cpp_dsp_showcase VERSION 0.3.${PATCH_VERSION}
                 DESCRIPTION "Modern C++ DSP showcase with tests, benchmarks, and CI"
                 LANGUAGES C CXX)

option(DSP_ENABLE_AVX2 "Enable AVX2 SIMD kernels where available" OFF)
option(DSP_BUILD_DEMOS "Build demonstration executables" ON)
option(DSP_BUILD_BENCHMARKS "Build benchmark tools and reports" ON)
option(DSP_WARNINGS_AS_ERRORS "Treat compiler warnings as errors" OFF)
option(UPDATE_SUBMODULES "Update git submodules during configure" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

include(cmake/UpdateSubmodules.cmake)

include(CTest)
include(GNUInstallDirs)

add_subdirectory(external)
add_subdirectory(dsp_core)
