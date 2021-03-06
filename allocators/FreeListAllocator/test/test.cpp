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


// static void AllocateMemory(
//     FreeListAllocator & freeListAllocator, 
//     AllocationTestBlock_t& curr_block,
//     AllocationTestBlock_t* previous_block)
// {
//     Status_t status;
//     size_t previous_used;
//     align::alignment_t adjusted_alignment;
    
//     /* Perform next allocation */
//     status = freeListAllocator.Allocate(
//         curr_block.allocation_size, 
//         curr_block.alignment, 
//         &curr_block.allocation);
//     ASSERT_EQ(status,kStatusSuccess);
//     ASSERT_NE(curr_block.allocation, nullptr);

//     /* Check that the allocated address is what we'd expect 
//     *  (i.e. the previous allocated address, plus the memory previously allocated, 
//     *  plus the adjusted alignment).*/
//     if (previous_block->allocation == nullptr)
//     {
//         /* Calculate the adjusted alignment for the initial allocation */
//         FreeListAllocator::FreeListHeader * header_address = 
//             reinterpret_cast<FreeListAllocator::FreeListHeader*>(reinterpret_cast<uintptr_t>(curr_block.allocation) - sizeof(FreeListAllocator::FreeListHeader));
//         adjusted_alignment = header_address->alignment_offset;
//     }
//     else
//     {        
//         /* Calculate the adjusted alignment for the previous allocation */
//         adjusted_alignment = align::alignForwardAdjustmentWithHeader(
//             reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(previous_block->allocation) + previous_block->allocation_size), 
//             curr_block.alignment, 
//             curr_block.header_size);

//         ASSERT_EQ(
//             reinterpret_cast<uintptr_t>(previous_block->allocation) + adjusted_alignment + previous_block->allocation_size, 
//             reinterpret_cast<uintptr_t>(curr_block.allocation));
//     }
    
//     /* Ensure used size and remaining size are correct */
//     previous_used = freeListAllocator.InUseMemory();
//     ASSERT_EQ(freeListAllocator.RemainingMemory(), MEMORY_SIZE_INIT - previous_used);

//     /* Ensure used memory accounts for newly allocated memory plus adjusted alignment */
//     ASSERT_EQ(freeListAllocator.InUseMemory(), previous_used + curr_block.allocation_size + adjusted_alignment);
// }





TEST(FreelistAllocatorTest, Allocate) 
{ 
    FreeListAllocator freeListAllocator(MEMORY_SIZE_INIT);
    std::vector<AllocationTestBlock_t> test_blocks;
    
    // for (auto& pair : allocation_pairs)
    // {   
    //     AllocateMemory(freeListAllocator, test_blocks, pair)
    // }
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}