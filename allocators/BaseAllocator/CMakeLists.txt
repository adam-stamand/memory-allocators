cmake_minimum_required(VERSION 3.10)

project(base_allocator LANGUAGES C CXX)

include_directories(inc)
 
# Get sources for library
set(
  BASE_ALLOCATOR_SOURCES
      "src/BaseAllocator.cpp"
)

# Only build tests if testing is enabled and if not inside another project
if(CMAKE_PROJECT_NAME STREQUAL TOP_PROJECT_NAME AND BUILD_TESTING)
    add_subdirectory(test)
endif()

# Create a static library for other allocators to use
add_library(base_allocator_lib STATIC ${BASE_ALLOCATOR_SOURCES})