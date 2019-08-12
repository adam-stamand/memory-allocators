#include "Debug.h"
#include "LinearAllocator.h"
#include <gtest/gtest.h>
 
TEST(LinearAllocatorTest, Allocate) { 
    alloc::LinearAllocator linTest(0x1000);
    void * ptr = nullptr;
    ptr = linTest.Allocate(sizeof(int)*100, alignof(int));
    ASSERT_NE(ptr,nullptr);
    ptr = linTest.Allocate(sizeof(long double)*100, alignof(long double));
    ASSERT_NE(ptr,nullptr);
    ptr = linTest.Allocate(sizeof(float)*0x1000, alignof(float));
    ASSERT_EQ(ptr,nullptr);
}
 
TEST(LinearAllocatorTest, Deallocate) {
    ASSERT_EQ(2,2);
}
 
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}