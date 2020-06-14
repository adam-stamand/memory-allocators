allocatortest(
    EXEC_NAME base_allocator_test
    TEST_NAME BaseAllocatorTest
    SOURCES
        "test.cpp"
    LIBS
        gtest 
        gtest_main 
        base_allocator_lib 
)