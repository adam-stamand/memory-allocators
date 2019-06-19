#pragma once

#include <iostream>

namespace alloc{


class BaseAllocator
{
public:
    BaseAllocator();
    virtual ~BaseAllocator();
    
    virtual void * Allocate(size_t size) = 0;
    virtual void Deallocate(void * ptr) = 0;

    size_t GetSize();
    size_t GetUsed();

protected:
    void * m_start;
    size_t m_size;
    size_t m_used;
};

}