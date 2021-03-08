#include "StackAllocator.hpp"
#include "Debug.hpp"
#include <libalign.hpp>

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


Status_t StackAllocator::Allocate(size_t size, size_t alignment, void **ptr)
{   
    SPDLOG_DEBUG("Allocating: curr_used={}B allocating={}B alignment={}B",
            InUseMemory(), size, alignment);

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
    if (!EnoughMemory(size + alignment_adjustment))
    {
        SPDLOG_WARN("Failed to allocate: Not enough memory left");
        *ptr = nullptr;
        return kStatusOutOfMemory;
    }

    /* Adjust for alignment (with header) first and set the callers pointer*/
    *ptr = AdjustMemory(alignment_adjustment);
     
    /* Write the stack header contiguously previous to allocated memory */
    StackHeader* header = reinterpret_cast<StackHeader*>(reinterpret_cast<uintptr_t>(*ptr) - sizeof(StackHeader));
    header->alignment_offset = alignment_adjustment;

    /* Now adjust for the requested memory size */
    (void) AdjustMemory(size);

    SPDLOG_DEBUG("Allocation complete: allocated_address={} curr_address={} curr_used={}B",
                *ptr, current_address_ , InUseMemory());

    return kStatusSuccess;
}


Status_t StackAllocator::Deallocate(void * ptr) 
{ 
    SPDLOG_DEBUG("Deallocating: curr_used={}B deallocating={}B",
        InUseMemory(), reinterpret_cast<uintptr_t>(current_address_) - reinterpret_cast<uintptr_t>(ptr));
    
    if (ptr == nullptr)
    {
        SPDLOG_DEBUG("Failed to allocate: Invalid pointer");
        return kStatusInvalidParam;
    }

    /* Locate the stack header. It's kept contiguously ahead of block being deallocated */
    StackHeader * stack_header = reinterpret_cast<StackHeader*>(reinterpret_cast<uintptr_t>(ptr) - sizeof(StackHeader));
    
    /* Use the alignment offset to adjust to the end of previously allocated memory */
    signed long address_adjustment = SUB_POINTER_POINTER_UINT(current_address_, ptr) + stack_header->alignment_offset;
    (void) AdjustMemory(-1 * address_adjustment);

    SPDLOG_DEBUG("Deallocation complete: deallocated_address={} curr_address={} curr_used={}B",
                ptr, current_address_ , InUseMemory());

    return kStatusSuccess;
}


Status_t StackAllocator::ClearMemory()
{
    Finalize();
    current_address_ = nullptr;
    SPDLOG_DEBUG("Memory cleared: curr_address={}", 
                 current_address_);
    return kStatusSuccess;
}


Status_t StackAllocator::InitMemory(size_t memory_size)
{
    BaseAllocator::Initialize(memory_size);
    current_address_ = GetAllocatorStart();
    SPDLOG_DEBUG("Memory Initialized: curr_address={}", 
                 current_address_);
    return kStatusSuccess;
}


} // namespace alloc