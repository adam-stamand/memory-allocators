#include "BaseAllocator.h"
#include "Debug.h"

using namespace alloc;

BaseAllocator::BaseAllocator()
{
    SPDLOG_DEBUG("Constructing");
}

BaseAllocator::~BaseAllocator()
{
    SPDLOG_DEBUG("Destructing");
}

size_t BaseAllocator::GetSize()
{
    SPDLOG_DEBUG("Getting size: {}", m_size);
    return m_size;
};

size_t BaseAllocator::GetUsed(){
    SPDLOG_DEBUG("Getting used: {}", m_used);
    return m_used;
}
