#include "LinearAllocator.h"
#include "Debug.h"

using namespace alloc;



LinearAllocator::LinearAllocator(size_t size){

    m_start = (void *)malloc(size);
    if (m_start == nullptr){
        SPDLOG_CRITICAL("LinearAllocator failed to malloc memory during construction");
        ASSERT(m_start);
    }
    m_size = size;
    m_used = 0;

};

LinearAllocator::~LinearAllocator(){
    free(m_start);
    m_size = 0;
    m_used = 0;
};

void * LinearAllocator::Allocate(size_t size){
    if (size > (m_size - m_used)){
        SPDLOG_WARN("LinearAllocator failed to allocate more memory; not enough memory left");
        return nullptr;
    }
    void * newBlock = (void*)(((uint8_t*)m_start) + m_used);
    m_used += size;
    SPDLOG_DEBUG("LinearAllocator allocated {} bytes", size);
    return newBlock;
}

void LinearAllocator::Deallocate(void * ptr){
    if ((ptr < m_start) || (ptr > (m_start + m_size))){
        // SPDLOG_CRITICAL("LinearAllocator deallocating memory that doesn't belong to it");
        // ASSERT((ptr < m_start) || (ptr > (m_start + m_size)));
    }

    Clear();
}

void LinearAllocator::Clear(){
    if (m_start == nullptr){
        SPDLOG_WARN("LinearAllocator clearing before any memory has been allocated");
    }
    free(m_start);
    m_size = 0;
    m_used = 0;
}