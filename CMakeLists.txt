cmake_minimum_required(VERSION 3.10)

# Disable in-source builds to prevent source tree corruption.
if(" ${CMAKE_SOURCE_DIR}" STREQUAL " ${CMAKE_BINARY_DIR}")
message(FATAL_ERROR "
FATAL: In-source builds are not allowed.
You should create a separate directory for build files.
(cd into build/ and run $cmake ..)
")
endif()

set(TOP_PROJECT_NAME memory_allocators)

project(${TOP_PROJECT_NAME} LANGUAGES C CXX)

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

########################################
# Set up the compiler flags
########################################
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0") # debug, no optimisation
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage") # enabling coverage
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)


if (NOT WIN32)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")
else(NOT WIN32)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
endif (NOT WIN32)

# Build tests. Remove 
include(CTest)

include_directories(include)
include_directories(utils/spdlog/include)
include_directories(utils)
include_directories(utils/gtest/googletest/include)
include_directories(utils/benchmark/include)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
 
set(
  memory_allocator_sources
      "${CMAKE_SOURCE_DIR}/src/BaseAllocator.cpp"
      "${CMAKE_SOURCE_DIR}/src/LinearAllocator.cpp"
      "${CMAKE_SOURCE_DIR}/example/main.cpp"
)
set(
  benchmark_sources
      "${CMAKE_SOURCE_DIR}/src/BaseAllocator.cpp"
      "${CMAKE_SOURCE_DIR}/src/LinearAllocator.cpp"
      "${CMAKE_SOURCE_DIR}/benchmark/benchmark.cpp"
)


if(CMAKE_PROJECT_NAME STREQUAL TOP_PROJECT_NAME AND BUILD_TESTING)
  # Prevent overriding the parent project's compiler/linker
  # settings on Windows
  set( gtest_force_shared_crt ON CACHE BOOL "Always use msvcrt.dll" FORCE)
  add_subdirectory(utils/gtest)
endif()

# Include all of the sub projects
add_subdirectory(allocators/BaseAllocator)
add_subdirectory(allocators/LinearAllocator)
add_subdirectory(example)