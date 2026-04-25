# Setting this will only affect the folders down from the current one
set(CMAKE_CXX_STANDARD 17)

if(BUILD_TESTING)
    set(BUILD_GMOCK OFF CACHE BOOL "Disable googlemock for this project" FORCE)
    set(INSTALL_GTEST OFF CACHE BOOL "Disable googletest install rules for this project" FORCE)
    add_subdirectory(googletest)
endif()
