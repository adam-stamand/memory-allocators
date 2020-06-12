#include "StackAllocator.h"
#include "Debug.h"
#include <libalign.h>

namespace alloc
{

#define MEM_PTR_ADD(op1, op2) ((void *)(((uint8_t *)op1) + op2))

StackAllocator::StackAllocator(size_t size) : 
BaseAllocator(size), current_address_(0)
{
    SPDLOG_DEBUG("Constructor called");
};

StackAllocator::~StackAllocator()
{
    SPDLOG_DEBUG("Destructor called");
};

AllocatorStatus_t StackAllocator::Allocate(size_t size, size_t alignment, void **ptr)
{
    SPDLOG_DEBUG("Allocating memory: size={}, alignment={}",
                 size, alignment);
    
    if (ptr == nullptr)
    {
        SPDLOG_DEBUG("Failed to allocate: Invalid pointer");
        return kStatusInvalidParam;
    }

    /* Next allocation will align according to specified alignment */
    size_t alignment_adjustment = align::alignForwardAdjustmentWithHeader(current_address_, alignment, sizeof(StackHeader));

    if (!IsEnoughMemory(size + alignment_adjustment))
    {
        SPDLOG_WARN("Failed to allocate more memory: Not enough memory left");
        ptr = nullptr;
        return kStatusOutOfMemory;
    }

    /* Write the stack header */
    header_address_ = reinterpret_cast<StackHeader*>(current_address_);
    header_address_->alignment_offset = alignment_adjustment;
    header_address_->previous_header = header_address_;

    /* Adjust for alignment first and set the callers pointer*/
    AddUsed(alignment_adjustment);
    *ptr = current_address_;

    /* Now allocate the requested memory size */
    AddUsed(size);

    return kStatusSuccess;
}

AllocatorStatus_t StackAllocator::Deallocate(void ** ptr) 
{ 
    SPDLOG_DEBUG("Deallocating memory");
    
    if (ptr == nullptr || ptr != reinterpret_cast<void*>(header_address_ + header_address_->alignment_offset))
    {
        SPDLOG_DEBUG("Failed to allocate: Invalid pointer");
        return kStatusInvalidParam;
    }
    StackHeader * header = reinterpret_cast<StackHeader *>(reinterpret_cast<uintptr_t>(ptr) - sizeof(StackHeader));
    current_address_ = ptr - header->alignment_offset;
}



void StackAllocator::AddUsed(size_t memory_used)
{
    SPDLOG_DEBUG("Adding to used: current={} adding={}", 
                 GetUsed(), memory_used);
    
    ASSERT(GetUsed() + memory_used <= GetAllocatorSize(), "Failed to add used: not enough memory left.");
    
    current_address_ = reinterpret_cast<void*>( reinterpret_cast<uintptr_t>(current_address_) + memory_used );
}

void StackAllocator::RemoveUsed(size_t memory_used)
{
    SPDLOG_DEBUG("Adding to used: current={} adding={}", 
                 GetUsed(), memory_used);
    
    ASSERT(GetUsed() + memory_used <= GetAllocatorSize(), "Failed to add used: not enough memory left.");
    
    current_address_ = reinterpret_cast<void*>( reinterpret_cast<uintptr_t>(current_address_) - memory_used );
}
} // namespace alloc