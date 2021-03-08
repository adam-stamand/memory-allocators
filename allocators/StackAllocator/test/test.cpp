#include "Debug.hpp"
#include "StackAllocator.hpp"
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
    // AllocationTestBlock_t * previous_block;
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


/***************
 * 
 * Local Functions
 * 
 * *****************/


static void AllocateMemory(
    StackAllocator & stackAllocator, 
    AllocationTestBlock_t& curr_block,
    AllocationTestBlock_t* previous_block)
{
    Status_t status;
    size_t previous_used;
    align::alignment_t adjusted_alignment;

    /* Ensure used size and remaining size are correct */
    previous_used = stackAllocator.InUseMemory();
    ASSERT_EQ(stackAllocator.RemainingMemory(), MEMORY_SIZE_INIT - previous_used);
    
    /* Perform next allocation */
    status = stackAllocator.Allocate(
        curr_block.allocation_size, 
        curr_block.alignment, 
        &curr_block.allocation);
    ASSERT_EQ(status,kStatusSuccess);
    ASSERT_NE(curr_block.allocation, nullptr);

    /* Check that the allocated address is what we'd expect 
    *  (i.e. the previous allocated address, plus the memory previously allocated, 
    *  plus the adjusted alignment).*/
    if (previous_block->allocation == nullptr)
    {
        /* Calculate the adjusted alignment for the initial allocation */
        StackAllocator::StackHeader * header_address = 
            reinterpret_cast<StackAllocator::StackHeader*>(reinterpret_cast<uintptr_t>(curr_block.allocation) - sizeof(StackAllocator::StackHeader));
        adjusted_alignment = header_address->alignment_offset;
    }
    else
    {        
        /* Calculate the adjusted alignment for the previous allocation */
        adjusted_alignment = align::alignForwardAdjustmentWithHeader(
            reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(previous_block->allocation) + previous_block->allocation_size), 
            curr_block.alignment, 
            curr_block.header_size);

        ASSERT_EQ(
            reinterpret_cast<uintptr_t>(previous_block->allocation) + adjusted_alignment + previous_block->allocation_size, 
            reinterpret_cast<uintptr_t>(curr_block.allocation));
    }
    
    /* Ensure used memory accounts for newly allocated memory plus adjusted alignment */
    ASSERT_EQ(stackAllocator.InUseMemory(), previous_used + curr_block.allocation_size + adjusted_alignment);
}

static void DeallocateMemory(
    StackAllocator & stackAllocator, 
    AllocationTestBlock_t& curr_block,
    AllocationTestBlock_t* previous_block)
{
    Status_t status;
    size_t previous_used;
    align::alignment_t adjusted_alignment;
    
    /* Ensure used size and remaining size are correct */
    previous_used = stackAllocator.InUseMemory();
    ASSERT_EQ(stackAllocator.RemainingMemory(), MEMORY_SIZE_INIT - previous_used);

    /* Deallocate the current block and ensure successful */
    status = stackAllocator.Deallocate(curr_block.allocation);
    ASSERT_EQ(status,kStatusSuccess);

    /* Calculate the adjusted alignment. If there's a previous block, use its allocation address
     * with the current alignment and header size to determine what adjusted alignment was used.
     * If no previous block, read the header directly to get the adjusted alignment*/
    if (previous_block->allocation == nullptr)
    {
        StackAllocator::StackHeader * header_address = 
            reinterpret_cast<StackAllocator::StackHeader*>(reinterpret_cast<uintptr_t>(curr_block.allocation) - sizeof(StackAllocator::StackHeader));
        adjusted_alignment = header_address->alignment_offset;

    }
    else
    {
        adjusted_alignment = align::alignForwardAdjustmentWithHeader(
            reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(previous_block->allocation) + previous_block->allocation_size), 
            curr_block.alignment, 
            curr_block.header_size);
    }

    ASSERT_EQ(stackAllocator.InUseMemory(), previous_used - curr_block.allocation_size - adjusted_alignment);
}

static void PushAllocateTestBlock(
    StackAllocator& stackAllocator,
    std::vector<AllocationTestBlock_t>& test_blocks,
    AllocationTestPair_t& pair
)
{
    /* Create test block */
    AllocationTestBlock_t block = 
    {
        .allocation_size = pair.allocation_size,
        .alignment = pair.alignment,
        .allocation = nullptr,
        .header_size = sizeof(align::alignment_t),
    };

    if (test_blocks.empty())
    {
        AllocationTestBlock_t init_block = 
        {        
            .allocation_size = 0,
            .alignment = 0,
            .allocation = nullptr,
            .header_size = 0,
        };

        AllocateMemory(
            stackAllocator, 
            block,
            &init_block);
    }
    else
    {
        AllocateMemory(
            stackAllocator, 
            block,
            &test_blocks.back());
    }

    test_blocks.push_back(block);
}

static void PopAllocateTestBlock(
    StackAllocator& stackAllocator,
    std::vector<AllocationTestBlock_t>& test_blocks
)
{
    AllocationTestBlock_t& block = test_blocks.back();
    test_blocks.pop_back();

    if (test_blocks.empty())
    {
        AllocationTestBlock_t init_block = 
        {        
            .allocation_size = 0,
            .alignment = 0,
            .allocation = nullptr,
            .header_size = 0,
        };

        DeallocateMemory(
            stackAllocator,
            block,
            &init_block);
    }
    else
    {
        DeallocateMemory(
            stackAllocator,
            block,
            &test_blocks.back());
    }
}


/***************
 * 
 * Tests
 * 
 * *****************/

TEST(StackAllocatorTest, Allocate) 
{ 
    StackAllocator stackAllocator(MEMORY_SIZE_INIT);
    std::vector<AllocationTestBlock_t> test_blocks;
    
    for (auto& pair : allocation_pairs)
    {   
        PushAllocateTestBlock(
            stackAllocator, 
            test_blocks, 
            pair);
    }
}
 
TEST(StackAllocatorTest, AllocateRandomly) 
{ 
    StackAllocator stackAllocator(MEMORY_SIZE_INIT);
    std::vector<AllocationTestBlock_t> test_blocks;
    
    srand( (unsigned)time(NULL) );
    for (unsigned int i = 0; i < MAX_RANDOM_TRIALS; i++)
    {   
        AllocationTestPair_t pair =
        {
            .allocation_size = static_cast<size_t>((rand() % 0x100u) + 1u),   
            .alignment= static_cast<align::alignment_t>((rand() % 128u) + 1u),
        };

        PushAllocateTestBlock(
            stackAllocator, 
            test_blocks, 
            pair);
    }
}

TEST(StackAllocatorTest, AllocateTooMuch) 
{ 
    StackAllocator stackAllocator(MEMORY_SIZE_INIT);
    Status_t status;
    void * curr_allocation;

    /* Ensure remaining size is init size */
    ASSERT_EQ(stackAllocator.RemainingMemory(), MEMORY_SIZE_INIT);
    
    /* Allocate more memory than available */
    status = stackAllocator.Allocate(
        MEMORY_SIZE_INIT + 1, 
        4, 
        &curr_allocation);
    EXPECT_EQ(status, kStatusOutOfMemory);

    /* Ensure remaining size is still init size */
    EXPECT_EQ(stackAllocator.RemainingMemory(), MEMORY_SIZE_INIT);    
}

TEST(StackAllocatorTest, AllocateInvalidInput) 
{ 
    StackAllocator stackAllocator(MEMORY_SIZE_INIT);
    Status_t status;
    void * curr_allocation;

    /* Ensure remaining size is init size */
    ASSERT_EQ(stackAllocator.RemainingMemory(), MEMORY_SIZE_INIT);
    
    /* Perform next allocation with bad pointer */
    status = stackAllocator.Allocate(
        0x10, 
        4, 
        nullptr);
    EXPECT_EQ(status, kStatusInvalidParam);

    /* Perform next allocation with bad alignment */
    status = stackAllocator.Allocate(
        0x10, 
        0, 
        &curr_allocation);
    EXPECT_EQ(status, kStatusInvalidParam);

    /* Perform next allocation with bad size */
    status = stackAllocator.Allocate(
        0, 
        4, 
        &curr_allocation);
    EXPECT_EQ(status, kStatusInvalidParam);

    /* Ensure remaining size is still init size */
    EXPECT_EQ(stackAllocator.RemainingMemory(), MEMORY_SIZE_INIT);    
}


TEST(StackAllocatorTest, Deallocate) 
{
    std::vector<AllocationTestBlock_t> test_blocks;
    StackAllocator stackAllocator(MEMORY_SIZE_INIT);

    /* Start with normal allocation */
    for (auto& pair : allocation_pairs)
    {   
        PushAllocateTestBlock(
            stackAllocator, 
            test_blocks, 
            pair);
    }

    /* Deallocate all blocks*/
    while (!test_blocks.empty())
    {   
        PopAllocateTestBlock(
            stackAllocator, 
            test_blocks);
    }

}

TEST(StackAllocatorTest, DeallocateRandomly) 
{
    std::vector<AllocationTestBlock_t> test_blocks;
    StackAllocator stackAllocator(MEMORY_SIZE_INIT);

    srand( (unsigned)time(NULL) );
    for (unsigned int i = 0; i < MAX_RANDOM_TRIALS; i++)
    {   
        AllocationTestPair_t pair =
        {
            .allocation_size = static_cast<size_t>((rand() % 0x100u) + 1u),   
            .alignment= static_cast<align::alignment_t>((rand() % 128u) + 1u),
        };

        PushAllocateTestBlock(
            stackAllocator, 
            test_blocks, 
            pair);
    }

    /* Deallocate all blocks*/
    while (!test_blocks.empty())
    {   
        PopAllocateTestBlock(
            stackAllocator, 
            test_blocks);
    }

}


TEST(StackAllocatorTest, ClearInit) 
{
    std::vector<AllocationTestBlock_t> test_blocks;
    StackAllocator stackAllocator(MEMORY_SIZE_INIT);
    size_t memory_size;
    bool is_enough;

    /* Start with normal allocation */
    for (auto& pair : allocation_pairs)
    {   
        PushAllocateTestBlock(
            stackAllocator, 
            test_blocks, 
            pair);
    }


    /* Clear the entire allocator and verify it's been cleared */
    stackAllocator.ClearMemory();
    memory_size = stackAllocator.InUseMemory();
    EXPECT_EQ(memory_size, 0u);
    memory_size = stackAllocator.TotalMemory();
    EXPECT_EQ(memory_size, 0u);
    memory_size = stackAllocator.RemainingMemory();
    EXPECT_EQ(memory_size, 0u);
    is_enough = stackAllocator.EnoughMemory(10);
    EXPECT_EQ(is_enough, false);
    is_enough = stackAllocator.EnoughMemory(0);
    EXPECT_EQ(is_enough, true); 

    /* Initialize the stack and ensure it's been initialized */
    stackAllocator.InitMemory(MEMORY_SIZE_INIT);
    memory_size = stackAllocator.InUseMemory();
    EXPECT_EQ(memory_size, 0u);
    memory_size = stackAllocator.TotalMemory();
    EXPECT_EQ(memory_size, MEMORY_SIZE_INIT);
    memory_size = stackAllocator.RemainingMemory();
    EXPECT_EQ(memory_size, MEMORY_SIZE_INIT);
    is_enough = stackAllocator.EnoughMemory(10);
    EXPECT_EQ(is_enough, true);

    /* Memory cleared, clear all old references */
    test_blocks.clear();
    
    /* Start with normal allocation */
    for (auto& pair : allocation_pairs)
    {   
        PushAllocateTestBlock(
            stackAllocator, 
            test_blocks, 
            pair);
    }

}
 

int main(int argc, char **argv) {
    spdlog::set_level(spdlog::level::debug); // Set global log level to debug
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}