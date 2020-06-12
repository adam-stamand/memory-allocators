cmake_minimum_required(VERSION 3.10)

project(alignment_test)

# Get the source files for testing
set(
  test_sources
      "test.cpp"
)

# Create an executable test
add_executable(alignment_test ${test_sources})

# Link the necessary libs (including base allocator)
target_link_libraries(alignment_test gtest gtest_main)

# Add the test so that CTest can be used for running
add_test(NAME AlignmentTest COMMAND alignment_test)