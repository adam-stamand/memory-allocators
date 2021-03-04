#pragma once

#include <libalign.hpp>
#include "BaseAllocator.hpp"

namespace alloc
{

/**
 * @brief Linear Allocator Class
 * 
 * This is the simplest kind of allocator. The idea is to keep a pointer 
 * at the first memory address of a memory chunk and move it forward every 
 * time an allocation is done. In this allocator, the internal 
 * fragmentation is kept to a minimum because all elements are sequentially 
 * (spatial locality) inserted and the only fragmentation between 
 * them is the alignment.
 * 
 * One of the drawbacks is that there is no deallocation. The only way to reuse 
 * the memory is to clear the entire allocator and start from the beginning.
 * 
 * This type of allocator is useful for cases when the lifetime of the allocated
 * memory is both relatively long, and the same for all users. For example, a new 
 * level in a video game could use a linear allocator for objects in that level.
 * When the level is over, the allocator can be cleared and reused for the next level.
 * 
 */
class LinearAllocator : public BaseAllocator
{
public:
  LinearAllocator(size_t memory_size);
  virtual ~LinearAllocator();

  /**
   * @brief Allocate Memory
   * 
   * @param size Size of the memory to be allocated
   * @param alignment Define byte boundry to align allocated memory to
   * @return void* Address of newly allocated memory
   */
  Status_t Allocate(size_t memory_size, size_t alignment, void **ptr);

  /**
   * @brief Deallocate Memory
   * 
   * Deallocation has no meaning in the context of a linear allocator.
   * The implementation of Deallocate just returns nullptr.
   * 
   * @param ptr Returns nullptr
   */
  Status_t Deallocate(void *ptr);


  bool EnoughMemory(size_t memory_size){return RemainingMemory() >= memory_size;}

  /**
   * @brief Clear Memory
   * 
   * Linear allocators can only be cleared. This Clear()
   * method releases all allocated memory and resets members
   * to initial state (0).
   * 
   */
  Status_t ClearMemory();

    
  Status_t InitMemory(size_t memory_size);

private:
    inline void* AdjustMemory(size_t memory_used){ AdjustMemoryInUse(memory_used); AdjustCurrentAddress(memory_used); return current_address_; }
    inline void AdjustCurrentAddress(size_t adjustment){ current_address_ = ADD_TO_POINTER(current_address_, adjustment); }
    void * current_address_;
};

} // namespace alloc
