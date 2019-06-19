#include <iostream>
#include "Debug.h"
#include "LinearAllocator.h"

using namespace alloc;


class TEST{

public:
  TEST(){}

};


int main(void){

  LinearAllocator linAlloc(0x100);
  int * my_arr = (int*)linAlloc.Allocate(0x30);
  return 0;
}
