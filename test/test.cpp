#include "LinearAllocator.h"
#include <gtest/gtest.h>
 
TEST(LinearAllocatorTest, Allocate) { 
    ASSERT_EQ(1,1);
}
 
TEST(LinearAllocatorTest, Deallocate) {
    ASSERT_EQ(2,2);
}
 
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}