#include "Debug.h"
#include "LinearAllocator.h"
#include <gtest/gtest.h>
 
using namespace alloc;

TEST(LinearAllocatorTest, Allocate) 
{ 
    AllocatorStatus_t status;
    const size_t MEMORY_SIZE_INIT = 0x1000;
    void * ptr = nullptr;
    void * previous_address;
    size_t previous_allocate_size = 0;
    size_t allocate_size = 0;
    size_t used_size;
    alloc::LinearAllocator linAllocator(MEMORY_SIZE_INIT);
    
    used_size = linAllocator.GetUsed();
    ASSERT_EQ(used_size,0);

    allocate_size = 0x13;
    status = linAllocator.Allocate(allocate_size, 1, &ptr);
    ASSERT_EQ(status,kStatusSuccess);
    ASSERT_NE(ptr,nullptr);
    ASSERT_EQ(linAllocator.GetUsed(), used_size + allocate_size);
    used_size = linAllocator.GetUsed();
    ASSERT_EQ(linAllocator.GetRemaining(), MEMORY_SIZE_INIT - used_size);
    previous_address = ptr;    
    previous_allocate_size = 0x13;
    
    allocate_size = 0x20;
    status = linAllocator.Allocate(allocate_size, 8, &ptr);
    ASSERT_EQ(status,kStatusSuccess);
    ASSERT_EQ(reinterpret_cast<uintptr_t>(previous_address) + previous_allocate_size + 5, reinterpret_cast<uintptr_t>(ptr));
    ASSERT_EQ(linAllocator.GetUsed(), used_size + allocate_size + 5);
    used_size = linAllocator.GetUsed();
    ASSERT_EQ(linAllocator.GetRemaining(), MEMORY_SIZE_INIT - used_size);
    previous_address = ptr;
    previous_allocate_size = allocate_size;


    allocate_size = 0x23;
    status = linAllocator.Allocate(allocate_size, 2, &ptr);
    ASSERT_EQ(status,kStatusSuccess);
    ASSERT_EQ(reinterpret_cast<uintptr_t>(previous_address) + previous_allocate_size, reinterpret_cast<uintptr_t>(ptr));
    ASSERT_EQ(linAllocator.GetUsed(), used_size + allocate_size);
    used_size = linAllocator.GetUsed();
    ASSERT_EQ(linAllocator.GetRemaining(), MEMORY_SIZE_INIT - used_size);
    previous_address = ptr;
    previous_allocate_size = allocate_size;
    
    allocate_size = 0x20;
    status = linAllocator.Allocate(allocate_size, 16, &ptr);
    ASSERT_EQ(status,kStatusSuccess);
    ASSERT_EQ(reinterpret_cast<uintptr_t>(previous_address) + previous_allocate_size + 5, reinterpret_cast<uintptr_t>(ptr));
    ASSERT_EQ(linAllocator.GetUsed(), used_size + allocate_size + 5);
    used_size = linAllocator.GetUsed();
    ASSERT_EQ(linAllocator.GetRemaining(), MEMORY_SIZE_INIT - used_size);
    previous_address = ptr;
    previous_allocate_size = allocate_size;
    
    allocate_size = 0x3;
    status = linAllocator.Allocate(allocate_size, 8, &ptr);
    ASSERT_EQ(status,kStatusSuccess);
    ASSERT_EQ(reinterpret_cast<uintptr_t>(previous_address) + previous_allocate_size, reinterpret_cast<uintptr_t>(ptr));
    ASSERT_EQ(linAllocator.GetUsed(), used_size + allocate_size);
    used_size = linAllocator.GetUsed();
    ASSERT_EQ(linAllocator.GetRemaining(), MEMORY_SIZE_INIT - used_size);
    previous_address = ptr;
    previous_allocate_size = allocate_size;
    
    allocate_size = 0x8;
    status = linAllocator.Allocate(allocate_size, 4, &ptr);
    ASSERT_EQ(status,kStatusSuccess);
    ASSERT_EQ(reinterpret_cast<uintptr_t>(previous_address) + previous_allocate_size + 1, reinterpret_cast<uintptr_t>(ptr));
    ASSERT_EQ(linAllocator.GetUsed(), used_size + allocate_size + 1);
    used_size = linAllocator.GetUsed();
    ASSERT_EQ(linAllocator.GetRemaining(), MEMORY_SIZE_INIT - used_size);
    previous_address = ptr;
    previous_allocate_size = allocate_size;

    allocate_size = 0;
    status = linAllocator.Allocate(allocate_size, 4, &ptr);
    ASSERT_EQ(status,kStatusSuccess);
    ASSERT_EQ(reinterpret_cast<uintptr_t>(previous_address) + previous_allocate_size, reinterpret_cast<uintptr_t>(ptr));
    ASSERT_EQ(linAllocator.GetUsed(), used_size + allocate_size);
    used_size = linAllocator.GetUsed();
    ASSERT_EQ(linAllocator.GetRemaining(), MEMORY_SIZE_INIT - used_size);
    previous_address = ptr;
    previous_allocate_size = allocate_size;

    allocate_size = 0x8;
    status = linAllocator.Allocate(allocate_size, 0, &ptr);
    ASSERT_EQ(status,kStatusSuccess);
    ASSERT_EQ(reinterpret_cast<uintptr_t>(previous_address), reinterpret_cast<uintptr_t>(ptr));
    ASSERT_EQ(linAllocator.GetUsed(), used_size + allocate_size);
    used_size = linAllocator.GetUsed();
    ASSERT_EQ(linAllocator.GetRemaining(), MEMORY_SIZE_INIT - used_size);
    previous_address = ptr;
    previous_allocate_size = allocate_size;

    status = linAllocator.Allocate(0x1000, 2, &ptr);
    ASSERT_EQ(status,kStatusOutOfMemory);

    status = linAllocator.Allocate(sizeof(float)*0x1000, alignof(float), nullptr);
    ASSERT_EQ(status,kStatusInvalidParam);

}
 


TEST(LinearAllocatorTest, ClearInitialize) 
{
    AllocatorStatus_t status;
    void * ptr = nullptr;
    void * previous_address;
    size_t previous_allocate_size = 0;
    size_t allocate_size = 0;
    size_t used_size;
    bool is_enough;
    alloc::LinearAllocator linAllocator(0x1000);
    
    used_size = linAllocator.GetUsed();
    ASSERT_EQ(used_size,0);

    allocate_size = 0x13;
    status = linAllocator.Allocate(allocate_size, 1, &ptr);
    ASSERT_EQ(status,kStatusSuccess);
    ASSERT_NE(ptr,nullptr);
    ASSERT_EQ(linAllocator.GetUsed(), used_size + allocate_size);
    used_size = linAllocator.GetUsed();
    previous_address = ptr;    
    previous_allocate_size = 0x13;
    
    allocate_size = 0x20;
    status = linAllocator.Allocate(allocate_size, 8, &ptr);
    ASSERT_EQ(status,kStatusSuccess);
    ASSERT_EQ(reinterpret_cast<uintptr_t>(previous_address) + previous_allocate_size + 5, reinterpret_cast<uintptr_t>(ptr));
    ASSERT_EQ(linAllocator.GetUsed(), used_size + allocate_size + 5);
    used_size = linAllocator.GetUsed();
    previous_address = ptr;
    previous_allocate_size = allocate_size;

    linAllocator.Clear();
    used_size = linAllocator.GetUsed();
    EXPECT_EQ(used_size, 0);
    allocate_size = linAllocator.GetAllocatorSize();
    EXPECT_EQ(allocate_size, 0);
    allocate_size = linAllocator.GetRemaining();
    EXPECT_EQ(allocate_size, 0);
    is_enough = linAllocator.IsEnoughMemory(10);
    EXPECT_EQ(is_enough, false);
    is_enough = linAllocator.IsEnoughMemory(0);
    EXPECT_EQ(is_enough, true);
    status = linAllocator.Deallocate(nullptr);
    EXPECT_EQ(status, kStatusFailure);   

    linAllocator.Initialize(0x1000);
    used_size = linAllocator.GetUsed();
    EXPECT_EQ(used_size, 0);
    allocate_size = linAllocator.GetAllocatorSize();
    EXPECT_EQ(allocate_size, 0x1000);
    allocate_size = linAllocator.GetRemaining();
    EXPECT_EQ(allocate_size, 0x1000);
    is_enough = linAllocator.IsEnoughMemory(10);
    EXPECT_EQ(is_enough, true);
    status = linAllocator.Deallocate(nullptr);
    EXPECT_EQ(status, kStatusFailure);   
}
 
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}