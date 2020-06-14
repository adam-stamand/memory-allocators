allocator(
    PROJ_NAME linear_allocator
    LIB_NAME linear_allocator_lib
    TEST_DIR test/
    SOURCES
        "src/LinearAllocator.cpp"
    LIBS
        base_allocator_lib 
    INC
        inc/
        ${CMAKE_SOURCE_DIR}/allocators/BaseAllocator/inc
)