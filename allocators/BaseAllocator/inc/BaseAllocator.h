#pragma once

#include <iostream>
#include "status.h"

namespace alloc{



class BaseAllocator
{
public:
    BaseAllocator(size_t memory_size);
    virtual ~BaseAllocator();
    
    virtual AllocatorStatus_t Allocate(size_t size, size_t alignment, void ** ptr) = 0;
    virtual AllocatorStatus_t Deallocate(void * ptr) = 0;

    /**
     * @brief Get the Allocator Size
     * 
     * @return size_t Size of all memory owned by allocator
     */
    inline size_t GetAllocatorSize() {return memory_size_;}

protected:

    /**
     * @brief Get the Allocator start address
     * 
     * @return void* Address of the memory owned by allocator
     */
    inline void* GetAllocatorStart() {return start_address_;}

    /**
   * @brief Init Allocator
   * 
   * Initializing the linear allocator will malloc a chunk of memory
   * that will then be allocated in subsequent Allocate() calls.
   * 
   * Initialization will fail if the allocator already owns memory. To
   * re-initialize, Finalize() must first be called.
   * 
   */
    void Initialize(size_t memory_size);

    /**
   * @brief Finalize Memory
   * 
   * Finalizing the allocator will free all memory owned by
   * the allocator and set the members to zero.
   * 
   */
    void Finalize();


private:
    size_t memory_size_;
    void * start_address_;
};

}