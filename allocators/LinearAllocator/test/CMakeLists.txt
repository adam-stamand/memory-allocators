allocatortest(
    EXEC_NAME linear_allocator_test
    TEST_NAME LinearAllocatorTest
    SOURCES
        "test.cpp"
    LIBS
        gtest 
        gtest_main 
        base_allocator_lib 
        linear_allocator_lib
)