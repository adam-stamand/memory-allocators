cmake_minimum_required(VERSION 3.10)

# Disable in-source builds to prevent source tree corruption.
if(" ${CMAKE_SOURCE_DIR}" STREQUAL " ${CMAKE_BINARY_DIR}")
message(FATAL_ERROR "
FATAL: In-source builds are not allowed.
You should create a separate directory for build files.
(cd into build/ and run $cmake ..)
")
endif()

project(memory_allocators LANGUAGES C CXX)

# set default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "" FORCE)
endif()


# output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin") 

# cmake config options
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set up the compiler flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0") # debug, no optimisation
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage") # enabling coverage
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)


if (NOT WIN32)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Werror")
else(NOT WIN32)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 -Werror")
endif (NOT WIN32)


include(CTest)
include_directories(lib)
include_directories(lib/libalign/inc)
include_directories(lib/libllist/inc)
include_directories(utils/spdlog/include)
include_directories(utils)
include_directories(utils/gtest/googletest/include)
include_directories(utils/benchmark/include)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})


if(BUILD_TESTING)
  # Prevent overriding the parent project's compiler/linker
  # settings on Windows
  set( gtest_force_shared_crt ON CACHE BOOL "Always use msvcrt.dll" FORCE)
  add_subdirectory(utils/gtest)
  set (TEST_TARGET "all_tests")
  add_custom_target(${TEST_TARGET} ALL)
endif()

# Include helper scripts
set(cmake_dirs "templates")
foreach(dir ${cmake_dirs})
    file(GLOB scripts "cmake/${dir}/*.cmake")
    foreach(script ${scripts})
        include(${script})
    endforeach()
endforeach()

set( CTEST_MEMORYCHECK_COMMAND valgrind )

# Include all of the sub projects
add_subdirectory(lib/libalign)
add_subdirectory(lib/libllist)
add_subdirectory(allocators/BaseAllocator)
add_subdirectory(allocators/LinearAllocator)
add_subdirectory(allocators/StackAllocator)
add_subdirectory(allocators/FreeListAllocator)