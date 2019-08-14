#!/usr/bin/env bash

  # Check versions of gcc, g++ and cmake
  gcc -v && g++ -v && cmake --version
  # Run your build commands next
  mkdir -p build && cd build
  cmake .. \
      --warn-uninitialized \
      -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
      -DCMAKE_CXX_STANDARD=$CPP
  cmake --build .
  echo $COVERITY_SCAN_TOKEN
  cd bin
  ls -l
  ./runTests
  valgrind ./memory_allocator