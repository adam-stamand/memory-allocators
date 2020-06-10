#pragma once

#include "BaseAllocator.h"

namespace alloc
{

/**
 * @brief Linear Allocator Class
 * 
 * This is the simplest kind of allocator. The idea is to keep a pointer 
 * at the first memory address of your memory chunk and move it every 
 * time an allocation is done. In this allocator, the internal 
 * fragmentation is kept to a minimum because all elements are sequentially 
 * (spatial locality) inserted and the only fragmentation between 
 * them is the alignment.
 * 
 */
class LinearAllocator : public BaseAllocator
{
public:
  LinearAllocator(size_t size);
  virtual ~LinearAllocator();

  /**
   * @brief Allocate Memory
   * 
   * @param size Size of the memory to be allocated
   * @param alignment Define byte boundry to align allocated memory to
   * @return void* Address of newly allocated memory
   */
  void *Allocate(size_t size, uint8_t alignment);

  /**
   * @brief Deallocate Memory
   * 
   * Deallocation has no meaning in the context of a linear allocator.
   * The implementation of Deallocate just returns nullptr.
   * 
   * @param ptr Returns nullptr
   */
  void Deallocate(void *ptr);

  /**
   * @brief Clear Memory
   * 
   * Linear allocators can only be cleared. This Clear()
   * method releases all allocated memory and resets members
   * to initial state (0).
   * 
   */
  void Clear();
};

} // namespace alloc
