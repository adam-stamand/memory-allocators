#include "Debug.hpp"
#include "FreeListAllocator.hpp"
#include <libalign.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <stdlib.h>
#include <time.h>

using namespace alloc;

/***************
 * 
 * Variables
 * 
 * *****************/

static const size_t MAX_RANDOM_TRIALS = 100;
static const size_t MAX_TRIAL_SIZE = 0x100;
static const size_t MEMORY_SIZE_INIT = MAX_RANDOM_TRIALS * MAX_TRIAL_SIZE;
 
 struct AllocationTestBlock_t
{
    size_t allocation_size;
    align::alignment_t alignment;
    void * allocation;
    align::alignment_t header_size;
};

struct AllocationTestPair_t
{
    size_t allocation_size;
    align::alignment_t alignment;
};

static std::vector<AllocationTestPair_t> allocation_pairs =
{
    {.allocation_size = 0x2,   .alignment=118},
    {.allocation_size = 0x13,   .alignment=7},
    {.allocation_size = 0x124,  .alignment=8},
    {.allocation_size = 0x1,    .alignment=24},
    {.allocation_size = 0x35,   .alignment=1},
    {.allocation_size = 0x74,   .alignment=4},
    {.allocation_size = 0x7,    .alignment=8},
};





TEST(FreelistAllocatorTest, Allocate) 
{ 
    FreeListAllocator freeListAllocator(MEMORY_SIZE_INIT);
    std::vector<AllocationTestBlock_t> test_blocks;
    
    // for (auto& pair : allocation_pairs)
    // {   

    // }
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}