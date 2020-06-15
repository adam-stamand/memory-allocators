allocatortest(
    EXEC_NAME free_list_allocator_test
    TEST_NAME FreeListAllocatorTest
    SOURCES
        "test.cpp"
    LIBS
        gtest 
        gtest_main 
        base_allocator_lib 
        free_list_allocator_lib
)