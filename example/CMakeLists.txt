cmake_minimum_required(VERSION 3.10)

project(example LANGUAGES C CXX)

include_directories(inc)
include_directories(${CMAKE_SOURCE_DIR}/allocators/LinearAllocator/inc)
include_directories(${CMAKE_SOURCE_DIR}/allocators/BaseAllocator/inc)

# Get sources for library
set(
    EXAMPLE_SOURCES
      "src/main.cpp"
)

# Build example executable
add_executable(example ${EXAMPLE_SOURCES})

# Make sure libs are built first
add_dependencies(example linear_allocator_test)

# Link the necessary libs (linear allocator and base_allocator)
target_link_libraries(example linear_allocator_lib base_allocator_lib)
