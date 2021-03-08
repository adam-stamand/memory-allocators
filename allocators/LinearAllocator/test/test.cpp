#include "Debug.hpp"
#include "LinearAllocator.hpp"
#include <gtest/gtest.h>
#include <libalign.hpp>
 
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
    LinearAllocator & linearAllocator, 
    AllocationTestBlock_t& curr_block,
    AllocationTestBlock_t* previous_block)
{
    Status_t status;
    size_t previous_used;
    align::alignment_t adjusted_alignment;

    /* Ensure used size and remaining size are correct */
    previous_used = linearAllocator.InUseMemory();
    ASSERT_EQ(linearAllocator.RemainingMemory(), MEMORY_SIZE_INIT - previous_used);
    
    /* Perform next allocation */
    status = linearAllocator.Allocate(
        curr_block.allocation_size, 
        curr_block.alignment, 
        &curr_block.allocation);
    ASSERT_EQ(status,kStatusSuccess);

    /* Check that the allocated address is what we'd expect 
    *  (i.e. the previous allocated address, plus the memory previously allocated, 
    *  plus the adjusted alignment). Initial allocation is not checked for correct
    *  size, since there's not way to know the alignment adjustment*/
    if (previous_block->allocation == nullptr)
    {
        ASSERT_NE(curr_block.allocation, nullptr);
    }
    else
    {        
        /* Calculate the adjusted alignment for the previous allocation */
        adjusted_alignment = align::alignForwardAdjustment(
            reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(previous_block->allocation) + previous_block->allocation_size), 
            curr_block.alignment);

        ASSERT_EQ(
            reinterpret_cast<uintptr_t>(previous_block->allocation) + adjusted_alignment + previous_block->allocation_size, 
            reinterpret_cast<uintptr_t>(curr_block.allocation));
        
        /* Ensure used memory account for newly allocated memory plus adjusted alignment */
        ASSERT_EQ(linearAllocator.InUseMemory(), previous_used + curr_block.allocation_size + adjusted_alignment);
    }
    
}

static void DeallocateMemory(
    LinearAllocator & linearAllocator, 
    AllocationTestBlock_t& curr_block,
    AllocationTestBlock_t* previous_block)
{
    Status_t status;
    
    status = linearAllocator.Deallocate(nullptr);
    ASSERT_EQ(status, kStatusFailure);
}

static void PushAllocateTestBlock(
    LinearAllocator& linearAllocator,
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
            linearAllocator, 
            block,
            &init_block);
    }
    else
    {
        AllocateMemory(
            linearAllocator, 
            block,
            &test_blocks.back());
    }

    test_blocks.push_back(block);
}

static void PopAllocateTestBlock(
    LinearAllocator& linearAllocator,
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
            linearAllocator,
            block,
            &init_block);
    }
    else
    {
        DeallocateMemory(
            linearAllocator,
            block,
            &test_blocks.back());
    }
}


/***************
 * 
 * Tests
 * 
 * *****************/

TEST(LinearAllocatorTest, Allocate) 
{ 
    LinearAllocator linearAllocator(MEMORY_SIZE_INIT);
    std::vector<AllocationTestBlock_t> test_blocks;
    
    for (auto& pair : allocation_pairs)
    {   
        PushAllocateTestBlock(
            linearAllocator, 
            test_blocks, 
            pair);
    }
}
 
TEST(LinearAllocatorTest, AllocateRandomly) 
{ 
    LinearAllocator linearAllocator(MEMORY_SIZE_INIT);
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
            linearAllocator, 
            test_blocks, 
            pair);
    }
}

TEST(LinearAllocatorTest, AllocateTooMuch) 
{ 
    LinearAllocator linearAllocator(MEMORY_SIZE_INIT);
    Status_t status;
    void * curr_allocation;

    /* Ensure remaining size is init size */
    ASSERT_EQ(linearAllocator.RemainingMemory(), MEMORY_SIZE_INIT);
    
    /* Allocate more memory than available */
    status = linearAllocator.Allocate(
        MEMORY_SIZE_INIT + 1, 
        4, 
        &curr_allocation);
    EXPECT_EQ(status, kStatusOutOfMemory);

    /* Ensure remaining size is still init size */
    EXPECT_EQ(linearAllocator.RemainingMemory(), MEMORY_SIZE_INIT);    
}

TEST(LinearAllocatorTest, AllocateInvalidInput) 
{ 
    LinearAllocator linearAllocator(MEMORY_SIZE_INIT);
    Status_t status;
    void * curr_allocation;

    /* Ensure remaining size is init size */
    ASSERT_EQ(linearAllocator.RemainingMemory(), MEMORY_SIZE_INIT);
    
    /* Perform next allocation with bad pointer */
    status = linearAllocator.Allocate(
        0x10, 
        4, 
        nullptr);
    EXPECT_EQ(status, kStatusInvalidParam);

    /* Perform next allocation with bad alignment */
    status = linearAllocator.Allocate(
        0x10, 
        0, 
        &curr_allocation);
    EXPECT_EQ(status, kStatusInvalidParam);

    /* Perform next allocation with bad size */
    status = linearAllocator.Allocate(
        0, 
        4, 
        &curr_allocation);
    EXPECT_EQ(status, kStatusInvalidParam);

    /* Ensure remaining size is still init size */
    EXPECT_EQ(linearAllocator.RemainingMemory(), MEMORY_SIZE_INIT);    
}



TEST(LinearAllocatorTest, Deallocate) 
{
    std::vector<AllocationTestBlock_t> test_blocks;
    LinearAllocator linearAllocator(MEMORY_SIZE_INIT);

    /* Start with normal allocation */
    for (auto& pair : allocation_pairs)
    {   
        PushAllocateTestBlock(
            linearAllocator, 
            test_blocks, 
            pair);
    }

    /* Deallocate all blocks*/
    while (!test_blocks.empty())
    {   
        PopAllocateTestBlock(
            linearAllocator, 
            test_blocks);
    }

}

TEST(LinearAllocatorTest, DeallocateRandomly) 
{
    std::vector<AllocationTestBlock_t> test_blocks;
    LinearAllocator linearAllocator(MEMORY_SIZE_INIT);

    srand( (unsigned)time(NULL) );
    for (unsigned int i = 0; i < MAX_RANDOM_TRIALS; i++)
    {   
        AllocationTestPair_t pair =
        {
            .allocation_size = static_cast<size_t>((rand() % 0x100u) + 1u),   
            .alignment= static_cast<align::alignment_t>((rand() % 128u) + 1u),
        };

        PushAllocateTestBlock(
            linearAllocator, 
            test_blocks, 
            pair);
    }

    /* Deallocate all blocks*/
    while (!test_blocks.empty())
    {   
        PopAllocateTestBlock(
            linearAllocator, 
            test_blocks);
    }

}

TEST(LinearAllocatorTest, ClearInit) 
{
    std::vector<AllocationTestBlock_t> test_blocks;
    LinearAllocator linearAllocator(MEMORY_SIZE_INIT);
    size_t memory_size;
    bool is_enough;

    /* Start with normal allocation */
    for (auto& pair : allocation_pairs)
    {   
        PushAllocateTestBlock(
            linearAllocator, 
            test_blocks, 
            pair);
    }


    /* Clear the entire allocator and verify it's been cleared */
    linearAllocator.ClearMemory();
    memory_size = linearAllocator.InUseMemory();
    EXPECT_EQ(memory_size, 0u);
    memory_size = linearAllocator.TotalMemory();
    EXPECT_EQ(memory_size, 0u);
    memory_size = linearAllocator.RemainingMemory();
    EXPECT_EQ(memory_size, 0u);
    is_enough = linearAllocator.EnoughMemory(10);
    EXPECT_EQ(is_enough, false);
    is_enough = linearAllocator.EnoughMemory(0);
    EXPECT_EQ(is_enough, true); 

    /* Initialize the linear and ensure it's been initialized */
    linearAllocator.InitMemory(MEMORY_SIZE_INIT);
    memory_size = linearAllocator.InUseMemory();
    EXPECT_EQ(memory_size, 0u);
    memory_size = linearAllocator.TotalMemory();
    EXPECT_EQ(memory_size, MEMORY_SIZE_INIT);
    memory_size = linearAllocator.RemainingMemory();
    EXPECT_EQ(memory_size, MEMORY_SIZE_INIT);
    is_enough = linearAllocator.EnoughMemory(10);
    EXPECT_EQ(is_enough, true);

    /* Memory cleared, clear all old references */
    test_blocks.clear();
    
    /* Start with normal allocation */
    for (auto& pair : allocation_pairs)
    {   
        PushAllocateTestBlock(
            linearAllocator, 
            test_blocks, 
            pair);
    }

}
 
 
int main(int argc, char **argv) {
    spdlog::set_level(spdlog::level::debug); // Set global log level to debug
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}