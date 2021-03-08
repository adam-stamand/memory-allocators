#include "FreeListAllocator.hpp"
#include "Debug.hpp"
#include <libalign.hpp>

namespace alloc
{



FreeListAllocator::FreeListAllocator(size_t size) : 
BaseAllocator(size)
{
    SPDLOG_DEBUG("Constructor called");
    // llist::LListNode<BlockSize_t>* free_block = reinterpret_cast<llist::LListNode<BlockSize_t> *>(GetAllocatorStart());
    llist::LListNode<BlockSize_t> * free_block = new(GetAllocatorStart()) llist::LListNode<BlockSize_t>();         
    free_block->data_ = size;
    free_block_list_.InsertHead(free_block);
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
    
    /* Search for first free memory block that is of sufficient size (first-fit) */
    size_t adjusted_size = 0;
    Status_t status = kStatusOutOfMemory;
    for (auto& free_block : free_block_list_)
    {
        auto header_size = std::max(sizeof(FreeListHeader), sizeof(llist::LListNode<BlockSize_t>));
        align::alignment_t alignment_adjustment = align::alignForwardAdjustmentWithHeader(
            &free_block, 
            alignment, 
            header_size);
        
        adjusted_size = size + alignment_adjustment;
        
        if (free_block.data_ < adjusted_size)
        {
            /* The free block is not large enough; skip to the next one */
            continue;
        }
        else if (free_block.data_ - adjusted_size < MIN_BLOCK_SIZE)
        {
            /* Not enough memory for another free block; allocate the entire block*/
            free_block_list_.RemoveNode(&free_block);
            
            /* Write the alignment header. It's kept contiguously ahead of allocated block */
            FreeListHeader * free_list_header = reinterpret_cast<FreeListHeader*>(reinterpret_cast<uintptr_t>(&free_block) + alignment_adjustment - sizeof(FreeListHeader));
            free_list_header->alignment_offset = alignment_adjustment;
            free_list_header->size = free_block.data_;
        }
        else
        {  
            /* Enough memory for another free block; create another free block */
            llist::LListNode<size_t> * new_block = new(ADD_TO_POINTER(&free_block, adjusted_size)) llist::LListNode<size_t>(); 
            new_block->data_ = free_block.data_ - adjusted_size;
            
            free_block_list_.SwapNode(&free_block, new_block);

            /* Write the alignment header. It's kept contiguously ahead of allocated block */
            FreeListHeader * free_list_header = reinterpret_cast<FreeListHeader*>(reinterpret_cast<uintptr_t>(&free_block) + alignment_adjustment - sizeof(FreeListHeader));
            free_list_header->alignment_offset = alignment_adjustment;
            free_list_header->size = size;
        } 

        AdjustMemoryInUse(adjusted_size);
        *ptr = ADD_TO_POINTER(&free_block, alignment_adjustment);
        SPDLOG_DEBUG("Allocation complete: allocated_address={} curr_used={}B",
            reinterpret_cast<void*>(*ptr), InUseMemory());    
    
        status = kStatusSuccess;
        break;
    }

    return status;
}

Status_t FreeListAllocator::Deallocate(void * ptr) 
{ 
    SPDLOG_DEBUG("Deallocating: curr_used={}B deallocating={}",
        InUseMemory(), reinterpret_cast<void*>(ptr));

    if (ptr == nullptr)
    {
        SPDLOG_DEBUG("Failed to allocate: Invalid pointer");
        return kStatusInvalidParam;
    }

    /* Locate the alignment header. It's kept contiguously ahead of allocated block */
    FreeListHeader free_list_header = *reinterpret_cast<FreeListHeader*>(reinterpret_cast<uintptr_t>(ptr) - sizeof(FreeListHeader));
    
    /* Use the header to find beginning of block and get block size */
    llist::LListNode<BlockSize_t> * free_block = new(ADD_TO_POINTER(ptr, -1 * free_list_header.alignment_offset)) llist::LListNode<BlockSize_t>();
    free_block->data_ = free_list_header.size + free_list_header.alignment_offset;

    /* Find previous free block */
    auto next_iter = std::find_if(
        free_block_list_.begin(),
        free_block_list_.end(),
        [free_block](const llist::LListNode<BlockSize_t>& node){return (free_block < &node);}
    );

    /* Determine if blocks need to be coalesced */
    bool prev_coalesce;
    bool next_coalesce;
    if (next_iter == free_block_list_.end())
    {
        prev_coalesce = ADD_TO_POINTER(free_block_list_.GetTail(), free_block_list_.GetTail()->data_) == free_block;
        next_coalesce = false;
    }else if (!next_iter->prev())
    {
        prev_coalesce = false;
        next_coalesce = ADD_TO_POINTER(free_block, free_block->data_) == &(*next_iter);
    }else
    {
        prev_coalesce = ADD_TO_POINTER(next_iter->prev(), next_iter->prev()->data_) == free_block;
        next_coalesce = ADD_TO_POINTER(free_block, free_block->data_) == &(*next_iter);
    }

    /* Perform adding of free block (and coalescing if necessary) */
    if (!prev_coalesce && !next_coalesce)
    {
        if (next_iter == free_block_list_.end())
        {
            free_block_list_.InsertTail(free_block);
        }else
        {
            free_block_list_.InsertNodeBefore(&(*next_iter), free_block);
        }
    }else if(prev_coalesce && next_coalesce)
    {
        next_iter->prev()->data_ += free_block->data_;
        free_block = next_iter->prev();
        free_block->data_ += next_iter->data_ ;
        free_block_list_.RemoveNode(&(*next_iter));
    
    }else if (prev_coalesce)
    {
        if (!next_iter->prev())
        {
            free_block->data_ += next_iter->data_;
            free_block_list_.SwapNode(&(*next_iter), free_block);
        }else
        {
            next_iter->prev()->data_ += free_block->data_;
            free_block = next_iter->prev();
        }
    }else if (next_coalesce)
    {
        free_block->data_ += next_iter->data_;
        free_block_list_.SwapNode(&(*next_iter), free_block);
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