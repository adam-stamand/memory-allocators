#include "LinearAllocator.h"
#include "Debug.h"
#include <libalign.h>

namespace alloc
{

LinearAllocator::LinearAllocator(size_t size) : 
BaseAllocator(size)
{
    SPDLOG_DEBUG("Linear Allocator constructed");
    current_address_ = GetAllocatorStart();
};

LinearAllocator::~LinearAllocator()
{
    SPDLOG_DEBUG("Linear Allocator destructed");
};

AllocatorStatus_t LinearAllocator::Allocate(size_t size, size_t alignment, void **ptr)
{
    SPDLOG_DEBUG("LinearAllocator allocating memory: size={}, alignment={}",
                 size, alignment);
    
    if (ptr == nullptr)
    {
        SPDLOG_DEBUG("Failed to allocate: invalid pointer");
        return kStatusInvalidParam;
    }

    /* Next allocation will align according to specified alignment */
    size_t alignment_adjustment = align::alignForwardAdjustment(current_address_, alignment);

    if (!IsEnoughMemory(size + alignment_adjustment))
    {
        SPDLOG_WARN("LinearAllocator failed to allocate more memory; not enough memory left");
        ptr = nullptr;
        return kStatusOutOfMemory;
    }

    /* Adjust for alignment first and set the callers pointer*/
    AddUsed(alignment_adjustment);
    *ptr = current_address_;

    /* Now allocate the requested memory size */
    AddUsed(size);

    return kStatusSuccess;
}

AllocatorStatus_t LinearAllocator::Deallocate(void ** ptr) 
{ 
    SPDLOG_WARN("LinearAllocator has no deallocation implementation.");
    return kStatusFailure; 
}

void LinearAllocator::AddUsed(size_t memory_used)
{
    SPDLOG_DEBUG("Adding to used: current={} adding={}", 
                 GetUsed(), memory_used);
    
    ASSERT(GetUsed() + memory_used <= GetAllocatorSize(), "Failed to add used: not enough memory left.");
    
    current_address_ = reinterpret_cast<void*>( reinterpret_cast<uintptr_t>(current_address_) + memory_used );
}

void LinearAllocator::Clear()
{
    SPDLOG_DEBUG("Clearing used memory: size={}", 
                 GetUsed());
    
    Finalize();
    current_address_ = nullptr;
}

void LinearAllocator::Initialize(size_t memory_size)
{
    SPDLOG_DEBUG("Initializing memory: size={}", 
                 memory_size);
    
    BaseAllocator::Initialize(memory_size);
    current_address_ = GetAllocatorStart();
}

} // namespace alloc