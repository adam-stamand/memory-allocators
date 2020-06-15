#pragma once

#include <libalign.h>
#include <libllist.h>
#include "BaseAllocator.h"

namespace alloc
{

typedef size_t BlockSize_t;

/**
 * @brief Free List Allocator Class
 *
 * 
 */
class FreeListAllocator : public BaseAllocator
{
public:
  FreeListAllocator(size_t memory_size);
  virtual ~FreeListAllocator();

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


  bool EnoughMemory(size_t memory_size){return true;}
  Status_t ClearMemory(); 
  Status_t InitMemory(size_t memory_size);


  // TODO make private (needed for testing)
  struct FreeListHeader
  {
    align::alignment_t alignment_offset;
  };

private:
 
  llist::LList<BlockSize_t> block_list_;
};

} // namespace alloc
