#include "BaseAllocator.h"
#include "Debug.h"

using namespace alloc;

BaseAllocator::BaseAllocator(){}

BaseAllocator::~BaseAllocator(){}

size_t BaseAllocator::GetSize(){
    return m_size;
};

size_t BaseAllocator::GetUsed(){
    return m_used;
}
