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
    virtual bool EnoughMemory(size_t memory_size) = 0;
    virtual AllocatorStatus_t ClearMemory() = 0;
    virtual AllocatorStatus_t InitMemory(size_t memory_size) = 0;

    size_t RemainingMemory(){return bytes_remaining_;}
    size_t InUseMemory(){return bytes_in_use_;}
    size_t TotalMemory(){return memory_size_;};

protected:
    inline void* GetAllocatorStart() {return start_address_;}
    void AdjustMemoryInUse(signed long memory_size);
    void Finalize();
    void Initialize(size_t memory_size);

private:
    size_t bytes_in_use_;
    size_t bytes_remaining_;
    size_t memory_size_;
    void * start_address_;
};

}