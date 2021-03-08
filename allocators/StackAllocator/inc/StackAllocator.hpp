#pragma once

#include "BaseAllocator.hpp"
#include <libalign.hpp>

namespace alloc
{



class StackAllocator : public BaseAllocator
{


public:
  StackAllocator(size_t memory_size);
  virtual ~StackAllocator();

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

  // TODO make private (needed for testing)
  struct StackHeader
  {
    align::alignment_t alignment_offset;
  };
  

private:
    inline void* AdjustMemory(size_t memory_used){ AdjustMemoryInUse(memory_used); AdjustCurrentAddress(memory_used); return current_address_;};
    inline void AdjustCurrentAddress(size_t adjustment){ current_address_ = ADD_TO_POINTER(current_address_, adjustment); }
    void * current_address_;
};




} // namespace alloc
