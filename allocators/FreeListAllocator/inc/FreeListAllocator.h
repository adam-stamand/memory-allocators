#pragma once

#include "BaseAllocator.h"
#include <libalign.h>

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
  AllocatorStatus_t Allocate(size_t memory_size, size_t alignment, void **ptr);

  /**
   * @brief Deallocate Memory
   * 
   * Deallocation has no meaning in the context of a linear allocator.
   * The implementation of Deallocate just returns nullptr.
   * 
   * @param ptr Returns nullptr
   */
  AllocatorStatus_t Deallocate(void *ptr);

  inline size_t GetUsed() { return reinterpret_cast<uintptr_t>(current_address_) - reinterpret_cast<uintptr_t>(GetAllocatorStart());}
  inline size_t GetRemaining() { return GetAllocatorSize() - GetUsed();}
  inline bool   IsEnoughMemory(size_t memory_size) {return (GetRemaining() >= memory_size);}

    /**
   * @brief Clear Memory
   * 
   * Linear allocators can only be cleared. This Clear()
   * method releases all allocated memory and resets members
   * to initial state (0).
   * 
   */
  void Clear();

    
  void Initialize(size_t memory_size);

  // TODO make private (needed for testing)
  struct StackHeader
  {
    align::alignment_t alignment_offset;
  };
  
private:




  void AddUsed(size_t memory_used);
  void * current_address_;
};




} // namespace alloc
