cmake_minimum_required(VERSION 3.10)

project(stack_allocator LANGUAGES C CXX)

include_directories(inc)
include_directories(${CMAKE_SOURCE_DIR}/allocators/BaseAllocator/inc)

# Get sources for library
set(
    STACK_ALLOCATOR_SOURCES
      "src/StackAllocator.cpp"
)

# Only build tests if testing is enabled and if not inside another project
if(CMAKE_PROJECT_NAME STREQUAL TOP_PROJECT_NAME AND BUILD_TESTING)
    add_subdirectory(test)
endif()

# Create a static library for other allocators to use
add_library(stack_allocator_lib STATIC ${STACK_ALLOCATOR_SOURCES})

# Make sure base allocator lib is built first
add_dependencies(stack_allocator_lib base_allocator_lib)

# Link the necessary libs (including base allocator and linear allocator)
target_link_libraries(stack_allocator_lib  base_allocator_lib )