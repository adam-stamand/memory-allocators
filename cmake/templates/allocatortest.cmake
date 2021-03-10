cmake_minimum_required(VERSION 3.10)

function(allocatortest)
    set(options "")
    set(single_value_args EXEC_NAME TEST_NAME)
    set(multi_value_args SOURCES FLAGS LIBS)
    cmake_parse_arguments(args "${options}" "${single_value_args}" "${multi_value_args}" ${ARGN})

    project(${args_EXEC_NAME})
    
    # Create an executable test
    add_executable(${args_EXEC_NAME} ${args_SOURCES})
    
    # Link the necessary libs (including base allocator and stack allocator)
    target_link_libraries(${args_EXEC_NAME} ${args_LIBS})
    
    # Add the test so that CTest can be used for running
    
    list(
        APPEND test_FLAGS
            "--gtest_color=yes"
            "--gtest_shuffle"
            "--gtest_output=xml:${CMAKE_BINARY_DIR}/reports/${test_TEST}.xml"
    )
    add_test(NAME ${args_TEST_NAME} COMMAND ${args_EXEC_NAME} ${test_FLAGS})
    add_test(NAME ${TEST_TARGET} COMMAND ${args_EXEC_NAME} ${test_FLAGS})
    
endfunction()
