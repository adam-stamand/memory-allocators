allocatortest(
    EXEC_NAME stack_allocator_test
    TEST_NAME StackAllocatorTest
    SOURCES
        "test.cpp"
    LIBS
        gtest 
        gtest_main 
        base_allocator_lib 
        stack_allocator_lib
)