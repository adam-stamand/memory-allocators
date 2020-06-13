cmake_minimum_required(VERSION 3.10)

project(stack_allocator_test)

# Get the source files for testing
set(
  test_sources
      "test.cpp"
)


# Create an executable test
add_executable(stack_allocator_test ${test_sources})

# Link the necessary libs (including base allocator and stack allocator)
target_link_libraries(stack_allocator_test gtest gtest_main base_allocator_lib stack_allocator_lib stack_allocator_lib)

# Add the test so that CTest can be used for running
add_test(NAME StackAllocatorTest COMMAND stack_allocator_test)