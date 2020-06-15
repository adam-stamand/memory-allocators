#include "FreeListAllocator.h"
#include "Debug.h"
#include <libalign.h>

namespace alloc
{

static bool FindGreaterEqualThan(llist::LListNode<size_t>* iterate_node, llist::LListNode<size_t>* find_node)
{return iterate_node->data_ >= find_node->data_ ;}


FreeListAllocator::FreeListAllocator(size_t size) : 
BaseAllocator(size)
{
    SPDLOG_DEBUG("Constructor called");
};

FreeListAllocator::~FreeListAllocator()
{
    SPDLOG_DEBUG("Destructor called");
};

Status_t FreeListAllocator::Allocate(size_t size, size_t alignment, void **ptr)
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

    while(true)
    {
        llist::LListNode<BlockSize_t> temp_node;
        llist::LListNode<BlockSize_t> * node_ptr;
        temp_node.data_ = size;
        node_ptr = block_list_.FindNode(FindGreaterEqualThan, &temp_node);

        if (node_ptr == nullptr)
        {
            SPDLOG_WARN("Failed to allocate: Not enough memory left");
            *ptr = nullptr;
            return kStatusOutOfMemory;
        }

        align::alignment_t alignment_adjustment = align::alignForwardAdjustmentWithHeader(
            ADD_TO_POINTER(node_ptr, sizeof(llist::LListNode<BlockSize_t>)), 
            alignment, 
            sizeof(FreeListHeader));
        if (node_ptr->data_ >= alignment_adjustment + size + sizeof(llist::LListNode<BlockSize_t>))
        {
            llist::LListNode<size_t> * new_block = 
                reinterpret_cast<llist::LListNode<size_t> *>(ADD_TO_POINTER(node_ptr, alignment_adjustment + size + sizeof(llist::LListNode<BlockSize_t>)));
            
            new_block->data_ = node_ptr->data_ - alignment_adjustment + size + sizeof(llist::LListNode<BlockSize_t>);
            
            /* Allocate memory and create a ne  w free block*/
            block_list_.InsertNodeAfter(node_ptr, new_block);
            break;
        }
    }

    /* Next allocation will align according to specified alignment */

    // if (!IsEnoughMemory(size + alignment_adjustment))
    // {
    //     SPDLOG_WARN("Failed to allocate: Not enough memory left");
    //     *ptr = nullptr;
    //     return kStatusOutOfMemory;
    // }

    // /* Adjust for alignment (with header) first and set the callers pointer*/
    // AddUsed(alignment_adjustment);
    // *ptr = current_address_;

    // /* Write the free_list header continuously previous to allocated memory */
    // FreeListHeader* header_address = reinterpret_cast<FreeListHeader*>(reinterpret_cast<uintptr_t>(current_address_) - sizeof(FreeListHeader));
    // header_address->alignment_offset = alignment_adjustment;

    // /* Now allocate the requested memory size */
    // AddUsed(size);

    // SPDLOG_DEBUG("Allocation complete: allocated_address={} curr_address={} curr_used={}B",
    //             *ptr, current_address_ , GetUsed());

    return kStatusSuccess;
}

Status_t FreeListAllocator::Deallocate(void * ptr) 
{ 
    // SPDLOG_DEBUG("Deallocating: curr_used={}B deallocating={}B",
    //     GetUsed(), reinterpret_cast<uintptr_t>(current_address_) - reinterpret_cast<uintptr_t>(ptr));
    
    // if (ptr == nullptr)
    // {
    //     SPDLOG_DEBUG("Failed to allocate: Invalid pointer");
    //     return kStatusInvalidParam;
    // }

    // /* Locate the free_list header. It's kept contiguously ahead of block being deallocated */
    // FreeListHeader * free_list_header = reinterpret_cast<FreeListHeader*>(reinterpret_cast<uintptr_t>(ptr) - sizeof(FreeListHeader));
    
    // /* Use the alignment offset to locate the end of previously allocated memory */
    // current_address_ = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(ptr) - free_list_header->alignment_offset);

    // SPDLOG_DEBUG("Deallocation complete: deallocated_address={} curr_address={} curr_used={}B",
    //             ptr, current_address_ , GetUsed());

    return kStatusSuccess;
}



// void FreeListAllocator::AddUsed(size_t memory_used)
// {
    
//     ASSERT(GetUsed() + memory_used <= GetAllocatorSize(), "Failed to add used: not enough memory left.");

//     current_address_ = reinterpret_cast<void*>( reinterpret_cast<uintptr_t>(current_address_) + memory_used );
// }

// void FreeListAllocator::Clear()
// {
//     Finalize();
//     current_address_ = nullptr;
//     SPDLOG_DEBUG("Memory cleared: curr_address={}", 
//                  current_address_);
// }

// void FreeListAllocator::Initialize(size_t memory_size)
// {
//     BaseAllocator::Initialize(memory_size);
//     current_address_ = GetAllocatorStart();
//     SPDLOG_DEBUG("Memory Initialized: curr_address={}", 
//                  current_address_);
// }

} // namespace alloc