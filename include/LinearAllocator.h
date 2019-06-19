#pragma once


#include "BaseAllocator.h"



namespace alloc{

class LinearAllocator : public BaseAllocator
{
public:
  LinearAllocator(size_t size);
  virtual ~LinearAllocator();

  void * Allocate(size_t size);
  void Deallocate(void * ptr);

private:
  void Clear();

};

}

