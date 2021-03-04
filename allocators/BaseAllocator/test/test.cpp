#include "Debug.hpp"
#include "BaseAllocator.hpp"
#include <gtest/gtest.h>
using namespace alloc;


static const size_t MEMORY_SIZE_INIT = 0x1000;

class TestAllocator : public alloc::BaseAllocator
{
public:
    TestAllocator(size_t memory_size):BaseAllocator(memory_size){}
    Status_t Allocate(size_t size, size_t alignment, void ** ptr){return kStatusSuccess;}
    Status_t Deallocate(void * ptr){return kStatusSuccess;}
    bool EnoughMemory(size_t memory_size){return kStatusSuccess;}
    Status_t ClearMemory(){return kStatusSuccess;}
    Status_t InitMemory(size_t memory_size){return kStatusSuccess;}
};

TEST(BaseAllocatorTest, Allocate) { 
    TestAllocator testAllocator(MEMORY_SIZE_INIT);
    
    Status_t status = testAllocator.Allocate(0, 0, nullptr);
    ASSERT_EQ(status, kStatusSuccess);
}


TEST(BaseAllocatorTest, Deallocate) {
    TestAllocator testAllocator(MEMORY_SIZE_INIT);
    
    Status_t status = testAllocator.Deallocate(nullptr);
    ASSERT_EQ(status, kStatusSuccess);
}

TEST(BaseAllocatorTest, GetAllocatorSize) { 
    TestAllocator testAllocator(MEMORY_SIZE_INIT);
    
    size_t allocator_size = testAllocator.TotalMemory();
    ASSERT_EQ(allocator_size, MEMORY_SIZE_INIT);
}
 
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}