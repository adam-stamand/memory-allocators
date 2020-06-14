cmake_minimum_required(VERSION 3.10)

function(allocator)
    set(options "")
    set(single_value_args PROJ_NAME LIB_NAME TEST_DIR)
    set(multi_value_args SOURCES LIBS INC DEP)
    cmake_parse_arguments(args "${options}" "${single_value_args}" "${multi_value_args}" ${ARGN})
    

    project(${args_PROJ_NAME} LANGUAGES C CXX)
    
    include_directories(${args_INC})

    # Only build tests if testing is enabled and if not inside another project
    if(CMAKE_PROJECT_NAME STREQUAL TOP_PROJECT_NAME AND BUILD_TESTING)
        add_subdirectory(${args_TEST_DIR})
    endif()
    
    
    # Create a static library for other allocators to use
    add_library(${args_LIB_NAME} STATIC ${args_SOURCES})

    
    if(args_LIBS)
        # Make sure other allocators are built first
        add_dependencies(${args_LIB_NAME} ${args_LIBS})
    
        # Link the necessary libs
        target_link_libraries(${args_LIB_NAME}  ${args_LIBS})
    endif()

endfunction()
