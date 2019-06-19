cmake_minimum_required(VERSION 2.8.9)
# Disable in-source builds to prevent source tree corruption.
if(" ${CMAKE_SOURCE_DIR}" STREQUAL " ${CMAKE_BINARY_DIR}")
  message(FATAL_ERROR "
FATAL: In-source builds are not allowed.
       You should create a separate directory for build files.
       (cd into build/ and run $cmake ..)
")
endif()

project (memory_allocator_proj)


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -g")

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

include_directories(include)
include_directories(include/spdlog/include)
include_directories(utils/spdlog/include)
include_directories(utils)

file(GLOB SOURCES "src/*.cpp")
message("  Sources: '${SOURCES}'")
add_executable(memory_allocator ${SOURCES})




