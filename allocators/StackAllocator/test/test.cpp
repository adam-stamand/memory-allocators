#include "Debug.h"
#include "StackAllocator.h"
#include <gtest/gtest.h>
 
using namespace alloc;

TEST(StackAllocatorTest, Allocate) { 
    AllocatorStatus_t status;
    void * ptr = nullptr;

    alloc::StackAllocator stackAllocator(0x1000);
    
    status = stackAllocator.Allocate(sizeof(int)*100, alignof(int), &ptr);
    ASSERT_EQ(status,kStatusSuccess);
    ASSERT_NE(ptr,nullptr);
    ptr = nullptr;
    
    status = stackAllocator.Allocate(sizeof(long double)*100, alignof(long double), &ptr);
    ASSERT_EQ(status,kStatusSuccess);
    ASSERT_NE(ptr,nullptr);
    ptr = nullptr;
    
    status = stackAllocator.Allocate(sizeof(float)*0x1000, alignof(float), &ptr);
    ASSERT_EQ(status,kStatusOutOfMemory);
    ASSERT_EQ(ptr,nullptr);

    size_t memory_size = stackAllocator.GetUsed();
    ASSERT_EQ(memory_size,  sizeof(long double)*100 +  sizeof(int)*100);

    status = stackAllocator.Allocate(sizeof(float)*0x1000, alignof(float), nullptr);
    ASSERT_EQ(status,kStatusInvalidParam);
    ASSERT_EQ(ptr,nullptr);
    
}
 


TEST(StackAllocatorTest, Deallocate) {
    ASSERT_EQ(2,2);
}
 
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}