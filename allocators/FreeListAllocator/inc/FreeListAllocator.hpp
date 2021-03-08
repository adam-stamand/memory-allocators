#pragma once

#include <libalign.hpp>
#include <libllist.hpp>
#include <algorithm>
#include "BaseAllocator.hpp"

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


  bool EnoughMemory(size_t memory_size){
    auto iter = std::find_if(
      free_block_list_.begin(), 
      free_block_list_.end(), 
      [memory_size](const llist::LListNode<BlockSize_t>& node){return node.data_ >= memory_size;});

    return iter != free_block_list_.end();
  }
  Status_t ClearMemory(); 
  Status_t InitMemory(size_t memory_size);


  // TODO make private (needed for testing)
  struct FreeListHeader
  {
    BlockSize_t size; // Size of the allocated memory (i.e. excludes header and offset)
    align::alignment_t alignment_offset; //> Number of bytes between beginning of free block and allocated address
  };

  static const size_t MIN_BLOCK_SIZE = sizeof(FreeListHeader) + sizeof(llist::LListNode<BlockSize_t>) + 1;

  void PrintFreeBlockList(){free_block_list_.PrintList();}
private:
 
  llist::LList<BlockSize_t> free_block_list_;
};

} // namespace alloc
