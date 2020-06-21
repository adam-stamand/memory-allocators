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
  
  static bool FindGreaterEqualThan(llist::LListNode<size_t>* iterate_node, size_t& data) {return iterate_node->data_ >= data ;}
  static bool FindSameNode(llist::LListNode<size_t>* iterate_node, void* find_node) {return iterate_node == find_node;}


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


  bool EnoughMemory(size_t memory_size){return free_block_list_.FindNode(FindGreaterEqualThan, memory_size) != nullptr;}
  Status_t ClearMemory(); 
  Status_t InitMemory(size_t memory_size);


  // TODO make private (needed for testing)
  struct FreeListHeader
  {
    BlockSize_t size;
    align::alignment_t alignment_offset;
  };

  static const size_t MIN_BLOCK_SIZE = sizeof(FreeListHeader) + sizeof(llist::LListNode<BlockSize_t>) + 1;

private:
 
  llist::LList<BlockSize_t> free_block_list_;
};

} // namespace alloc
