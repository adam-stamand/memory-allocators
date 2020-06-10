#include "Debug.h"
#include "BaseAllocator.h"
#include <gtest/gtest.h>
using namespace alloc;

class TestAllocator : public alloc::BaseAllocator
{
public:
    TestAllocator(){};
    void * Allocate(size_t size, uint8_t alignment)
    {
        this->m_size = size;
        this->m_used = 0;
        return 0;
    };
    void Deallocate(void * ptr)
    {
        this->m_size = 0;
    };
};

TEST(BaseAllocatorTest, Allocate) { 
    TestAllocator testAllocator;
    
    (void) testAllocator.Allocate(sizeof(int)*100, alignof(int));
    
    ASSERT_EQ(testAllocator.GetSize(),sizeof(int)*100);
}
 


TEST(BaseAllocatorTest, Deallocate) {
    TestAllocator testAllocator;

    (void) testAllocator.Deallocate(nullptr);
    
    ASSERT_EQ(testAllocator.GetSize(),(size_t)0);
}
 
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}