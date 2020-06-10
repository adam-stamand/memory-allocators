cmake_minimum_required(VERSION 3.10)

project(linear_allocator LANGUAGES C CXX)

include_directories(inc)
include_directories(${CMAKE_SOURCE_DIR}/allocators/BaseAllocator/inc)

# Get sources for library
set(
    LINEAR_ALLOCATOR_SOURCES
      "src/LinearAllocator.cpp"
)

# Only build tests if testing is enabled and if not inside another project
if(CMAKE_PROJECT_NAME STREQUAL TOP_PROJECT_NAME AND BUILD_TESTING)
    add_subdirectory(test)
endif()

# Make sure base allocator lib is built first
add_dependencies(linear_allocator_test base_allocator_lib)

# Create a static library for other allocators to use
add_library(linear_allocator_lib STATIC ${LINEAR_ALLOCATOR_SOURCES})