#include "BaseAllocator.h"
#include "Debug.h"

using namespace alloc;

BaseAllocator::BaseAllocator(size_t memory_size) : 
memory_size_(0),start_address_(nullptr)
{
    SPDLOG_DEBUG("Base Allocator constructed");
    
    Initialize(memory_size);
}

BaseAllocator::~BaseAllocator()
{
    SPDLOG_DEBUG("Base Allocator destroyed");

    Finalize();
}

void BaseAllocator::Initialize(size_t memory_size)
{
    SPDLOG_DEBUG("Initializing memory: size={}", memory_size);
    
    ASSERT(start_address_ == nullptr && memory_size_ == 0, "Invalid state for initialization");

    start_address_ = malloc(memory_size);
    ASSERT(start_address_ != nullptr, "Failed to malloc memory");
        
    memory_size_ = memory_size;
}

void BaseAllocator::Finalize()
{
    SPDLOG_DEBUG("Finalizing memory: size={}", 
                 memory_size_);
    
    free(start_address_);

    start_address_ = nullptr;
    memory_size_ = 0;
}