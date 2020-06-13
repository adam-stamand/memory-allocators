#include "LinearAllocator.h"
#include "Debug.h"
#include <libalign.h>

namespace alloc
{

LinearAllocator::LinearAllocator(size_t size) : 
BaseAllocator(size), current_address_(0)
{
    SPDLOG_DEBUG("Constructor called");
    current_address_ = GetAllocatorStart();
};

LinearAllocator::~LinearAllocator()
{
    SPDLOG_DEBUG("Destructor called");
};

AllocatorStatus_t LinearAllocator::Allocate(size_t size, size_t alignment, void **ptr)
{   
    SPDLOG_DEBUG("Allocating: curr_used={}B allocating={}B alignment={}B",
            GetUsed(), size, alignment);

    if (ptr == nullptr)
    {
        SPDLOG_DEBUG("Failed to allocate: Invalid pointer");
        return kStatusInvalidParam;
    }
    
    if (alignment == 0 || size == 0)
    {
        SPDLOG_DEBUG("Failed to allocate: Invalid alignment");
        return kStatusInvalidParam;
    }

    /* Next allocation will align according to specified alignment */
    align::alignment_t alignment_adjustment = align::alignForwardAdjustment(current_address_, alignment);

    if (!IsEnoughMemory(size + alignment_adjustment))
    {
        SPDLOG_WARN("Failed to allocate: Not enough memory left");
        *ptr = nullptr;
        return kStatusOutOfMemory;
    }

    /* Adjust for alignment (with header) first and set the callers pointer*/
    AddUsed(alignment_adjustment);
    *ptr = current_address_;

    /* Now allocate the requested memory size */
    AddUsed(size);

    SPDLOG_DEBUG("Allocation complete: allocated_address={} curr_address={} curr_used={}B",
                *ptr, current_address_ , GetUsed());

    return kStatusSuccess;
}
AllocatorStatus_t LinearAllocator::Deallocate(void * ptr) 
{ 
    SPDLOG_WARN("LinearAllocator has no deallocation implementation.");
    return kStatusFailure; 
}



void LinearAllocator::AddUsed(size_t memory_used)
{
    
    ASSERT(GetUsed() + memory_used <= GetAllocatorSize(), "Failed to add used: not enough memory left.");

    current_address_ = reinterpret_cast<void*>( reinterpret_cast<uintptr_t>(current_address_) + memory_used );
}

void LinearAllocator::Clear()
{
    Finalize();
    current_address_ = nullptr;
    SPDLOG_DEBUG("Memory cleared: curr_address={}", 
                 current_address_);
}

void LinearAllocator::Initialize(size_t memory_size)
{
    BaseAllocator::Initialize(memory_size);
    current_address_ = GetAllocatorStart();
    SPDLOG_DEBUG("Memory Initialized: curr_address={}", 
                 current_address_);
}


} // namespace alloc