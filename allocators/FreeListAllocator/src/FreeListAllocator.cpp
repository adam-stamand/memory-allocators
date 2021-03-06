#include "FreeListAllocator.hpp"
#include "Debug.hpp"
#include <libalign.hpp>

namespace alloc
{



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
        SPDLOG_DEBUG("Failed to allocate: Invalid alignment or size");
        return kStatusInvalidParam;
    }

    size_t adjusted_size = 0;
    
    /* Search for first free memory block that is of sufficient size (first-fit) */
    Status_t status = kStatusOutOfMemory;
    for (auto& free_block : free_block_list_)
    {
        align::alignment_t alignment_adjustment = align::alignForwardAdjustmentWithHeader(
            &free_block, 
            alignment, 
            sizeof(FreeListHeader));
        
        adjusted_size = size + alignment_adjustment;
        
        if (free_block.data_ < adjusted_size)
        {
            continue;
        }
        else if (free_block.data_ - adjusted_size < MIN_BLOCK_SIZE)
        {
            /* Not enough memory for another free block; allocate the entire block*/
            free_block_list_.RemoveNode(&free_block);
        }
        else
        {  
            /* Enough memory for another free block; create another free block */
            llist::LListNode<size_t> * new_block = 
                reinterpret_cast<llist::LListNode<size_t> *>(ADD_TO_POINTER(&free_block, adjusted_size));
            
            new_block->data_ = free_block.data_ - adjusted_size;
            
            free_block_list_.InsertNodeAfter(&free_block, new_block);
            free_block_list_.RemoveNode(&free_block);
        } 

        SPDLOG_DEBUG("Allocation complete: allocated_address={} curr_used={}B",
            reinterpret_cast<void*>(&free_block), InUseMemory());    
        AdjustMemoryInUse(adjusted_size);
        status = kStatusSuccess;
    }

    return status;
}

Status_t FreeListAllocator::Deallocate(void * ptr) 
{ 
    SPDLOG_DEBUG("Deallocating: curr_used={}B deallocating={}B",
        InUseMemory(), reinterpret_cast<uintptr_t>(ptr));

    if (ptr == nullptr)
    {
        SPDLOG_DEBUG("Failed to allocate: Invalid pointer");
        return kStatusInvalidParam;
    }

    /* Locate the alignment header. It's kept contiguously ahead of allocated block */
    FreeListHeader * free_list_header = reinterpret_cast<FreeListHeader*>(reinterpret_cast<uintptr_t>(ptr) - sizeof(FreeListHeader));
    
    /* Use the header to find beginning of block and get block size */
    llist::LListNode<BlockSize_t> * free_block = reinterpret_cast<llist::LListNode<BlockSize_t> *>(ADD_TO_POINTER(ptr, -1 * free_list_header->alignment_offset));
    free_block->data_ = free_list_header->size;

    /* Find previous free block */
    auto next_iter = std::find_if(
        free_block_list_.begin(),
        free_block_list_.end(),
        [free_block](const llist::LListNode<BlockSize_t>& node){return (free_block < &node);}
    );

    /* Next free block found; determine if necessary to coalesce */
    if (next_iter != free_block_list_.end())
    {
        /* Coalesce previous block (if contiguous) */
        if (ADD_TO_POINTER(next_iter->prev(), next_iter->prev()->data_) == free_block)
        {
            next_iter->prev()->data_ += free_block->data_;
            free_block = next_iter->prev();
        }

        /* Coalesce next block (if contiguous) */
        if (ADD_TO_POINTER(&(*next_iter), next_iter->data_) == free_block)
        {
            free_block->data_ += next_iter->data_ ;
            free_block_list_.InsertNodeBefore(&(*next_iter), free_block);
            free_block_list_.RemoveNode(&(*next_iter));
        }
    }

    AdjustMemoryInUse(-1 * free_block->data_);
    SPDLOG_DEBUG("Deallocation complete: deallocated_address={} curr_used={}B",
                ptr, InUseMemory());
    return kStatusSuccess;
}



Status_t FreeListAllocator::ClearMemory()
{
    BaseAllocator::Finalize();
    return kStatusSuccess;
}

Status_t FreeListAllocator::InitMemory(size_t memory_size)
{
    BaseAllocator::Initialize(memory_size);
    return kStatusSuccess;
}

} // namespace alloc