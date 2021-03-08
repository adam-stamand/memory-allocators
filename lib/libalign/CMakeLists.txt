cmake_minimum_required(VERSION 3.10)

project(libalign LANGUAGES C CXX)

include_directories(inc)
 

# Only build tests if testing is enabled and if not inside another project
if(CMAKE_PROJECT_NAME STREQUAL TOP_PROJECT_NAME AND BUILD_TESTING)
    add_subdirectory(test)
endif()

