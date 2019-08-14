cmake_minimum_required(VERSION 2.8.9)

# Disable in-source builds to prevent source tree corruption.
if(" ${CMAKE_SOURCE_DIR}" STREQUAL " ${CMAKE_BINARY_DIR}")
  message(FATAL_ERROR "
    FATAL: In-source builds are not allowed.
    You should create a separate directory for build files.
    (cd into build/ and run $cmake ..)
  ")
endif()

add_subdirectory(utils/gtest)

project (memory_allocators)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0") # debug, no optimisation
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage") # enabling coverage
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

if (NOT WIN32)
  # set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
  list(APPEND CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
  find_package (gtest REQUIRED)
  find_package (Threads)
  include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
  # message("Building for non Win32")
  # include(gtest.cmake)
  # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -g")
else(NOT WIN32)
  message("Building for Win32")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  # include_directories(C:/Users/adams/gtest/googletest/include)
  
  include_directories(utils/gtest/googletest/include)
endif (NOT WIN32)






# Locate GTest
# set(GTEST_ROOT "/usr/local/include" CACHE PATH "Path to googletest")
# set (GTEST_ROOT "/usr/local/include/gtest")
# find_package(GTest REQUIRED)

# Link runTests with what we want to test and the GTest and pthread library
include_directories(include)
include_directories(utils/spdlog/include)
include_directories(utils)

file(GLOB SOURCES "src/*.cpp")
message("  Sources: '${SOURCES}'")
add_executable(memory_allocator ${SOURCES})

file(GLOB SOURCES "src/LinearAllocator.cpp" "test/test.cpp" "src/BaseAllocator.cpp")
add_executable(runTests ${SOURCES})


if (NOT WIN32)
  target_link_libraries(runTests gtest ${CMAKE_THREAD_LIBS_INIT})
else (NOT WIN32)
  # target_link_libraries(runTests C:/Users/adams/gtest/googletest/build/Debug/gtestd.lib)
  target_link_libraries(runTests ../utils/gtest/googletest/build/Debug/gtestd)
endif (NOT WIN32)






