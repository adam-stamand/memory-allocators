#include "LinearAllocator.h"
#include "Debug.h"

namespace alloc
{

#define MEM_PTR_ADD(op1, op2) ((void *)(((uint8_t *)op1) + op2))

LinearAllocator::LinearAllocator(size_t size)
{

    m_start = (void *)malloc(size);
    if (m_start == nullptr)
    {
        ASSERT(m_start, "LinearAllocator failed to malloc memory during construction");
    }
    m_size = size;
    m_used = 0;
    SPDLOG_DEBUG("LinearAllocator created with size {} bytes", size);
};

LinearAllocator::~LinearAllocator()
{
    free(m_start);
    m_size = 0;
    m_used = 0;
    SPDLOG_DEBUG("LinearAllocator deleted");
};

void *LinearAllocator::Allocate(size_t size, uint8_t alignment)
{

    if (size > (m_size - m_used))
    {
        SPDLOG_WARN("LinearAllocator failed to allocate more memory; not enough memory left");
        return nullptr;
    }

    m_used += ((alignment - 1) & ~(alignment - 1));

    void *newBlock = MEM_PTR_ADD(m_start, m_used);
    m_used += size;
    SPDLOG_DEBUG("LinearAllocator allocated {} bytes, aligned to {} bytes", size, alignment);
    return newBlock;
}

void LinearAllocator::Deallocate(void *ptr) { return; }

void LinearAllocator::Clear()
{
    if (m_start == nullptr)
    {
        SPDLOG_WARN("LinearAllocator clearing before any memory has been allocated");
    }
    free(m_start);
    m_size = 0;
    m_used = 0;
    SPDLOG_DEBUG("LinearAllocator cleared");
}

} // namespace alloc