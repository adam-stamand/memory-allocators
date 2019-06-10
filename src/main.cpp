#include <iostream>
#include "spdlog/spdlog.h"
#include "LinearAllocator.h"


int main(void){
  puts("SDFSDFSDF");
  TestClass test;
  test.TestFunc(30);
  spdlog::info("Hello, {}!", "World");
  while (1);
  return 0;
}
