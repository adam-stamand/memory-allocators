cmake_minimum_required(VERSION 3.10)

# Create a test target to build all tests
add_custom_target(tests)

# Make sure base allocator lib is built first
add_dependencies(tests base_allocator_test linear_allocator_test stack_allocator_test alignment_test)

# Create a lib target to build all libs
add_custom_target(libs)

# Make sure base allocator lib is built first
add_dependencies(libs base_allocator_lib linear_allocator_lib stack_allocator_lib)