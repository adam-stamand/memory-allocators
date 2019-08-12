#include <iostream>
#include "Debug.h"
#include "LinearAllocator.h"

using namespace alloc;



int main(void){

  // TODO Figure out special functions
  LinearAllocator linAlloc(0x1000);
  
  // TODO use smart pointers? Typecast more smartly
  int * my_arr1 = (int*)linAlloc.Allocate(sizeof(int)*10, alignof(int));
  int * my_arr2 = (int*)linAlloc.Allocate(sizeof(double)*200, alignof(double));


  // TODO Move to GoogleTest
  for (int i = 0; i < 10; i++){
    my_arr1[i] = i;
  }

  for (int i = 0; i < 200; i++){
    my_arr2[i] = i;
  }

  

  return 0;
}
