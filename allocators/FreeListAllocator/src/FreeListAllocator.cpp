#include "StackAllocator.h"
#include "Debug.h"
#include <libalign.h>

namespace alloc
{

StackAllocator::StackAllocator(size_t size) : 
BaseAllocator(size), current_address_(0)
{
    SPDLOG_DEBUG("Constructor called");
    current_address_ = GetAllocatorStart();
};

StackAllocator::~StackAllocator()
{
    SPDLOG_DEBUG("Destructor called");
};

AllocatorStatus_t StackAllocator::Allocate(size_t size, size_t alignment, void **ptr)
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
    align::alignment_t alignment_adjustment = align::alignForwardAdjustmentWithHeader(current_address_, alignment, sizeof(StackHeader));

    if (!IsEnoughMemory(size + alignment_adjustment))
    {
        SPDLOG_WARN("Failed to allocate: Not enough memory left");
        *ptr = nullptr;
        return kStatusOutOfMemory;
    }

    /* Adjust for alignment (with header) first and set the callers pointer*/
    AddUsed(alignment_adjustment);
    *ptr = current_address_;

    /* Write the stack header continuously previous to allocated memory */
    StackHeader* header_address = reinterpret_cast<StackHeader*>(reinterpret_cast<uintptr_t>(current_address_) - sizeof(StackHeader));
    header_address->alignment_offset = alignment_adjustment;

    /* Now allocate the requested memory size */
    AddUsed(size);

    SPDLOG_DEBUG("Allocation complete: allocated_address={} curr_address={} curr_used={}B",
                *ptr, current_address_ , GetUsed());

    return kStatusSuccess;
}

AllocatorStatus_t StackAllocator::Deallocate(void * ptr) 
{ 
    SPDLOG_DEBUG("Deallocating: curr_used={}B deallocating={}B",
        GetUsed(), reinterpret_cast<uintptr_t>(current_address_) - reinterpret_cast<uintptr_t>(ptr));
    
    if (ptr == nullptr)
    {
        SPDLOG_DEBUG("Failed to allocate: Invalid pointer");
        return kStatusInvalidParam;
    }

    /* Locate the stack header. It's kept contiguously ahead of block being deallocated */
    StackHeader * stack_header = reinterpret_cast<StackHeader*>(reinterpret_cast<uintptr_t>(ptr) - sizeof(StackHeader));
    
    /* Use the alignment offset to locate the end of previously allocated memory */
    current_address_ = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(ptr) - stack_header->alignment_offset);

    SPDLOG_DEBUG("Deallocation complete: deallocated_address={} curr_address={} curr_used={}B",
                ptr, current_address_ , GetUsed());

    return kStatusSuccess;
}



void StackAllocator::AddUsed(size_t memory_used)
{
    
    ASSERT(GetUsed() + memory_used <= GetAllocatorSize(), "Failed to add used: not enough memory left.");

    current_address_ = reinterpret_cast<void*>( reinterpret_cast<uintptr_t>(current_address_) + memory_used );
}

void StackAllocator::Clear()
{
    Finalize();
    current_address_ = nullptr;
    SPDLOG_DEBUG("Memory cleared: curr_address={}", 
                 current_address_);
}

void StackAllocator::Initialize(size_t memory_size)
{
    BaseAllocator::Initialize(memory_size);
    current_address_ = GetAllocatorStart();
    SPDLOG_DEBUG("Memory Initialized: curr_address={}", 
                 current_address_);
}

} // namespace alloc