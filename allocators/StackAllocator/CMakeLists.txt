allocator(
    PROJ_NAME stack_allocator
    LIB_NAME stack_allocator_lib
    TEST_DIR test/
    SOURCES
        "src/StackAllocator.cpp"
    LIBS
        base_allocator_lib 
    INC
        inc/
        ${CMAKE_SOURCE_DIR}/allocators/BaseAllocator/inc
)