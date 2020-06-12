#include "Debug.h"
#include "BaseAllocator.h"
#include <gtest/gtest.h>
using namespace alloc;

class TestAllocator : public alloc::BaseAllocator
{
public:
    TestAllocator(size_t memory_size):BaseAllocator(memory_size){};
    
    AllocatorStatus_t Allocate(size_t size, size_t alignment, void ** ptr)
    {
        return kStatusSuccess;
    };

    AllocatorStatus_t Deallocate(void ** ptr)
    {
        return kStatusSuccess;
    };
};

TEST(BaseAllocatorTest, Allocate) { 
    TestAllocator testAllocator(0x1000);
    void * ptr;
    
    AllocatorStatus_t status = testAllocator.Allocate(sizeof(int)*100, alignof(int), &ptr);
    ASSERT_EQ(status, kStatusSuccess);
}
 


TEST(BaseAllocatorTest, Deallocate) {
    TestAllocator testAllocator(0x1000);
    
    AllocatorStatus_t status = testAllocator.Deallocate(nullptr);
    ASSERT_EQ(status, kStatusSuccess);
}
 
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}