# Adapted from https://cliutils.gitlab.io/modern-cmake/chapters/projects/submodule.html
if(NOT UPDATE_SUBMODULES)
    return()
endif()

find_package(Git QUIET)
if(NOT GIT_FOUND)
    message(STATUS "Git is not available. Skipping submodule update.")
    return()
endif()

if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/.git")
    message(STATUS "No .git directory detected. Skipping submodule update.")
    return()
endif()

execute_process(
    COMMAND ${GIT_EXECUTABLE} rev-parse --is-inside-work-tree
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    RESULT_VARIABLE GIT_CHECK_RETURN_CODE
    OUTPUT_QUIET
    ERROR_QUIET
)
if(NOT GIT_CHECK_RETURN_CODE EQUAL 0)
    message(STATUS "Not a git worktree. Skipping submodule update.")
    return()
endif()

execute_process(
    COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    RESULT_VARIABLE SUBMODULE_RETURN_CODE
)
if(NOT SUBMODULE_RETURN_CODE EQUAL 0)
    message(WARNING "Cannot update submodules. Git command failed with ${SUBMODULE_RETURN_CODE}")
else()
    message(STATUS "Git submodules updated successfully.")
endif()
