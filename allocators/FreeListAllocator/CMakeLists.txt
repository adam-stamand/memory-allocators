allocator(
    PROJ_NAME free_list_allocator
    LIB_NAME free_list_allocator_lib
    TEST_DIR test/
    SOURCES
        "src/FreeListAllocator.cpp"
    LIBS
        base_allocator_lib 
    INC
        inc/
        ${CMAKE_SOURCE_DIR}/allocators/BaseAllocator/inc
)