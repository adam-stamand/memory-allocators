cmake_minimum_required(VERSION 2.8.9)

# Disable in-source builds to prevent source tree corruption.
if(" ${CMAKE_SOURCE_DIR}" STREQUAL " ${CMAKE_BINARY_DIR}")
  message(FATAL_ERROR "
    FATAL: In-source builds are not allowed.
    You should create a separate directory for build files.
    (cd into build/ and run $cmake ..)
  ")
endif()


set(PROJECT_NAME memory_allocators)



project(${PROJECT_NAME})
# After project(..) is called, the following variables are available:
# * PROJECT_SOURCE_DIR - Top level source directory for the project
# * PROJECT_BINARY_DIR - Full path to build directory for project

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


# Link runTests with what we want to test and the GTest and pthread library
include_directories(include)
include_directories(utils/spdlog/include)
include_directories(utils)
include_directories(utils/gtest/googletest/include)
include_directories(utils/benchmark/include)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
 


file(GLOB SOURCES "src/*.cpp")
file(GLOB BENCH_SOURCES "benchmark/*.cpp" "src/BaseAllocator.cpp" "src/LinearAllocator.cpp")
file(GLOB TEST_SOURCES "src/LinearAllocator.cpp" "test/test.cpp" "src/BaseAllocator.cpp")
message("  Sources: '${SOURCES}'")
message("  Test Sources: '${TEST_SOURCES}'")
message("  Bench Sources: '${BENCH_SOURCES}'")


set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "enable X functionality" FORCE)
set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "enable X functionality" FORCE)
# Prevent overriding the parent project's compiler/linker
# settings on Windows
set( gtest_force_shared_crt ON CACHE BOOL "Always use msvcrt.dll" FORCE)
add_subdirectory(utils/benchmark)
add_subdirectory(utils/gtest)



enable_testing()

add_executable(memory_allocator ${SOURCES})
add_executable(memoryBenchmark ${BENCH_SOURCES})
add_executable(runTests ${TEST_SOURCES})

target_link_libraries(runTests gtest gtest_main)
target_link_libraries(memoryBenchmark benchmark)





