cmake_minimum_required(VERSION 3.10)

project(run_allocator_tests)


# Create an executable test
add_custom_target(run_allocator_tests)

# Make sure base allocator lib is built first
add_dependencies(run_allocator_tests base_allocator_test linear_allocator_test)