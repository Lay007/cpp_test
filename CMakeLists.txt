cmake_minimum_required(VERSION 3.10)
# Init pathch and project versions
set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.0.${PATCH_VERSION})

#project(helloworld VERSION ${PROJECT_VESRION})

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

project(try_googletest VERSION 0.0.1
                       DESCRIPTION "Try googletest"
                       LANGUAGES CXX)

# Update the submodules here
include(cmake/UpdateSubmodules.cmake)
# Enable testing for this project

# Add subdirectories with code
include(CTest)
add_subdirectory(external)
add_subdirectory(external/googletest)
add_subdirectory(my_project)

enable_testing()
find_package(GTest REQUIRED)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

configure_file(version.h.in version.h)

#add_executable(helloworld_cli main.cpp)
#add_library(helloworld lib.cpp)
#add_executable(test_version_gtest test_version_gtest.cpp)

#set_target_properties(helloworld_cli helloworld test_version_gtest PROPERTIES
#    CXX_STANDARD 14
#    CXX_STANDARD_REQUIRED ON
#)
#target_include_directories(helloworld
#    PRIVATE "${CMAKE_BINARY_DIR}"
#)

#target_link_libraries(helloworld_cli PRIVATE
#    helloworld
#)

#target_include_directories(test_version_gtest
#    PRIVATE ${GTEST_INCLUDE_DIRS}
#)
#target_link_libraries(
#    test_version_gtest
#    ${GTEST_BOTH_LIBRARIES}
#    helloworld
#)


#if (MSVC)
#    target_compile_options(helloworld_cli PRIVATE
#        /W4
#    ) 
#else ()
#    target_compile_options(helloworld_cli PRIVATE
#        -Wall -Wextra -pedantic -Werror
#    )
#endif()



#install(TARGETS helloworld_cli RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

#enable_testing()

#add_test(test_version_gtest test_version_gtest) 