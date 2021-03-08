#include "Debug.hpp"
#include "FreeListAllocator.hpp"
#include <libalign.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <iostream>
using namespace alloc;

/***************
 * 
 * Variables
 * 
 * *****************/

static const size_t MAX_RANDOM_TRIALS = 100;
static const size_t MAX_TRIAL_SIZE = 0x100;
static const size_t MEMORY_SIZE_INIT = MAX_RANDOM_TRIALS * MAX_TRIAL_SIZE;
 
class Test
{
public: 
    Test(){}

    virtual void Run(FreeListAllocator& freeListAllocator) = 0;
};

class AllocationRunner : public Test
{
public:
    AllocationRunner(size_t allocation_size, align::alignment_t alignment, AllocationRunner* prev_allocation):
        allocation_size_(allocation_size),alignment_(alignment),prev_allocation_(prev_allocation){}

    void Run(FreeListAllocator& freeListAllocator)
    {

        Status_t status;
        size_t previous_used;

        /* Ensure used size and remaining size are correct */
        previous_used = freeListAllocator.InUseMemory();
        ASSERT_EQ(freeListAllocator.RemainingMemory(), MEMORY_SIZE_INIT - previous_used);
        
        /* Perform next allocation */
        status = freeListAllocator.Allocate(
            allocation_size_, 
            alignment_, 
            &allocation_);
        ASSERT_EQ(status,kStatusSuccess);
        ASSERT_NE(allocation_, nullptr);
        if (prev_allocation_)
        {
            auto header_size = std::max(sizeof(FreeListAllocator::FreeListHeader), sizeof(llist::LListNode<BlockSize_t>));
            auto alignment_adjustment = align::alignForwardAdjustmentWithHeader(
                ADD_TO_POINTER(prev_allocation_->allocation_, prev_allocation_->allocation_size_), 
                alignment_, 
                header_size);
            ASSERT_EQ(ADD_TO_POINTER(prev_allocation_->allocation_, prev_allocation_->allocation_size_ + alignment_adjustment), allocation_);
        }
    }
    size_t allocation_size_;
    align::alignment_t alignment_;
    void* allocation_;
    AllocationRunner* prev_allocation_;
private:
};


class DeallocationRunner : public Test
{
public:
    DeallocationRunner(AllocationRunner* allocation_test):allocation_test_(allocation_test){}

    void Run(FreeListAllocator& freeListAllocator)
    {
        Status_t status;
        size_t previous_used;
        
        /* Ensure used size and remaining size are correct */
        previous_used = freeListAllocator.InUseMemory();
        ASSERT_EQ(freeListAllocator.RemainingMemory(), MEMORY_SIZE_INIT - previous_used);

        /* Deallocate the current block and ensure successful */
        status = freeListAllocator.Deallocate(allocation_test_->allocation_);
        ASSERT_EQ(status,kStatusSuccess);
    }
private:
    AllocationRunner* allocation_test_;
};


class TestCases
{
public:
    
    std::vector<Test*>& GetTestCases(){return tests;} 
protected:
    std::vector<Test*> tests;

};

class AllocateTest : public TestCases
{
public:
    
    AllocateTest()
    {
        tests.push_back(new AllocationRunner(8, 4, nullptr));
        tests.push_back(new AllocationRunner(8, 4, dynamic_cast<AllocationRunner*>(tests.at(0))));
        tests.push_back(new AllocationRunner(8, 4, dynamic_cast<AllocationRunner*>(tests.at(1))));
        tests.push_back(new AllocationRunner(8, 4, dynamic_cast<AllocationRunner*>(tests.at(2))));
        tests.push_back(new AllocationRunner(8, 13, dynamic_cast<AllocationRunner*>(tests.at(3))));
        tests.push_back(new AllocationRunner(7, 4, dynamic_cast<AllocationRunner*>(tests.at(4))));
        tests.push_back(new AllocationRunner(8, 4, dynamic_cast<AllocationRunner*>(tests.at(5))));
        tests.push_back(new AllocationRunner(8, 4, dynamic_cast<AllocationRunner*>(tests.at(6))));
    }
};

class UpperAndLowerCoalesceTest : public TestCases
{
public:
    
    UpperAndLowerCoalesceTest()
    {
        tests.push_back(new AllocationRunner(8, 4,nullptr));
        tests.push_back(new AllocationRunner(16, 4, dynamic_cast<AllocationRunner*>(tests.at(0))));
        tests.push_back(new AllocationRunner(4, 4, dynamic_cast<AllocationRunner*>(tests.at(1))));
        tests.push_back(new AllocationRunner(8, 4, dynamic_cast<AllocationRunner*>(tests.at(2))));
        tests.push_back(new AllocationRunner(16, 4, dynamic_cast<AllocationRunner*>(tests.at(3))));
        
        tests.push_back(new DeallocationRunner(dynamic_cast<AllocationRunner*>(tests.at(1))));
        tests.push_back(new DeallocationRunner(dynamic_cast<AllocationRunner*>(tests.at(3))));
        tests.push_back(new DeallocationRunner(dynamic_cast<AllocationRunner*>(tests.at(2))));
    
        // TODO
        // tests.push_back(new AllocationRunner(16, 4, dynamic_cast<AllocationRunner*>(tests.at(0))));
        // tests.push_back(new AllocationRunner(4, 4, dynamic_cast<AllocationRunner*>(tests.at(1))));
        // tests.push_back(new AllocationRunner(8, 4, dynamic_cast<AllocationRunner*>(tests.at(2))));
    
        // tests.push_back(new DeallocationRunner(dynamic_cast<AllocationRunner*>(tests.at(0))));
        // tests.push_back(new DeallocationRunner(dynamic_cast<AllocationRunner*>(tests.at(1))));
        // tests.push_back(new DeallocationRunner(dynamic_cast<AllocationRunner*>(tests.at(2))));
    
        // tests.push_back(new AllocationRunner(8, 4, nullptr));
        // tests.push_back(new AllocationRunner(16, 4, dynamic_cast<AllocationRunner*>(tests.at(0))));
        // tests.push_back(new AllocationRunner(4, 4, dynamic_cast<AllocationRunner*>(tests.at(1))));
    
        // tests.push_back(new DeallocationRunner(dynamic_cast<AllocationRunner*>(tests.at(2))));
        // tests.push_back(new DeallocationRunner(dynamic_cast<AllocationRunner*>(tests.at(4))));
        // tests.push_back(new DeallocationRunner(dynamic_cast<AllocationRunner*>(tests.at(3))));
    
    }

};



TEST(FreelistAllocatorTest, Allocate) 
{ 
    FreeListAllocator freeListAllocator(MEMORY_SIZE_INIT);
    AllocateTest test;
    
    for (auto& test : test.GetTestCases())
    {
        static int idx = 0;
        SPDLOG_DEBUG("Running test #{}", idx++);
        test->Run(freeListAllocator);
        freeListAllocator.PrintFreeBlockList();
    }
    
}


TEST(FreelistAllocatorTest, UpperAndLowerCoalesce) 
{ 
    FreeListAllocator freeListAllocator(MEMORY_SIZE_INIT);
    UpperAndLowerCoalesceTest test;
    
    for (auto& test : test.GetTestCases())
    {
        static int idx = 0;
        SPDLOG_DEBUG("Running test #{}", idx++);
        test->Run(freeListAllocator);
        freeListAllocator.PrintFreeBlockList();
    }
    
}


int main(int argc, char **argv) {
    spdlog::set_level(spdlog::level::debug); // Set global log level to debug
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}