#include "Debug.h"
#include "FreeListAllocator.h"
#include <libalign.h>
#include <gtest/gtest.h>
#include <vector>
#include <stdlib.h>
#include <time.h>

using namespace alloc;

/***************
 * 
 * Variables
 * 
 * *****************/

 

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}