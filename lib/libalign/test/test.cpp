#include "Debug.h"
#include <libalign.h>
#include <gtest/gtest.h>
using namespace align;



TEST(AlignmentTest, AlignAddress) { 
    void* address;
    
    address = alignForward((void*)0x102, 0x4);
    EXPECT_EQ(address, (void*)0x104);

    address = alignForward((void*)0x105, 0x2);
    EXPECT_EQ(address, (void*)0x106);

    address = alignForward((void*)0x111, 0x8);
    EXPECT_EQ(address, (void*)0x118);

    address = alignForward((void*)0x123, 0x10);
    EXPECT_EQ(address, (void*)0x130);

    address = alignForward((void*)0x245, 0x1);
    EXPECT_EQ(address, (void*)0x245);

    address = alignForward((void*)0x578, 0x8);
    EXPECT_EQ(address, (void*)0x578);

    address = alignForward((void*)0x632, 0x2);
    EXPECT_EQ(address, (void*)0x632);

    address = alignForward((void*)0x113, 0x1);
    EXPECT_EQ(address, (void*)0x113);

    address = alignForward((void*)0x352, 0x0);
    EXPECT_EQ(address, (void*)0);
}
 

 TEST(AlignmentTest, AlignAddressAdjustment) { 
    size_t adjustment;
    
    adjustment = alignForwardAdjustment((void*)0x102, 0x4);
    EXPECT_EQ(adjustment, 2u);

    adjustment = alignForwardAdjustment((void*)0x105, 0x2);
    EXPECT_EQ(adjustment, 1u);

    adjustment = alignForwardAdjustment((void*)0x111, 0x8);
    EXPECT_EQ(adjustment, 7u);

    adjustment = alignForwardAdjustment((void*)0x123, 0x10);
    EXPECT_EQ(adjustment, 13u);

    adjustment = alignForwardAdjustment((void*)0x245, 0x1);
    EXPECT_EQ(adjustment, 0u);

    adjustment = alignForwardAdjustment((void*)0x578, 0x8);
    EXPECT_EQ(adjustment, 0u);

    adjustment = alignForwardAdjustment((void*)0x632, 0x2);
    EXPECT_EQ(adjustment, 0u);

    adjustment = alignForwardAdjustment((void*)0x113, 0x1);
    EXPECT_EQ(adjustment, 0u);

    adjustment = alignForwardAdjustment((void*)0x352, 0x0);
    EXPECT_EQ(adjustment, 0u);
}
 

TEST(AlignmentTest, AlignAddressAdjustmentWithHeader) { 
    size_t adjustment;

    adjustment = alignForwardAdjustmentWithHeader((void*)0x102, 0x4, 0x5);
    EXPECT_EQ(adjustment, 6u);

    adjustment = alignForwardAdjustmentWithHeader((void*)0x105, 0x2, 0x10);
    EXPECT_EQ(adjustment, 17u);

    adjustment = alignForwardAdjustmentWithHeader((void*)0x111, 0x8, 0x5);
    EXPECT_EQ(adjustment, 7u);

    adjustment = alignForwardAdjustmentWithHeader((void*)0x123, 0x10, 0x20);
    EXPECT_EQ(adjustment, 45u);

    adjustment = alignForwardAdjustmentWithHeader((void*)0x245, 0x1, 0x5);
    EXPECT_EQ(adjustment, 5u);

    adjustment = alignForwardAdjustmentWithHeader((void*)0x578, 0x8, 0x9);
    EXPECT_EQ(adjustment, 16u);

    adjustment = alignForwardAdjustmentWithHeader((void*)0x632, 0x2, 0x05);
    EXPECT_EQ(adjustment, 6u);

    adjustment = alignForwardAdjustmentWithHeader((void*)0x113, 0x1, 0x03);
    EXPECT_EQ(adjustment, 3u);

    adjustment = alignForwardAdjustmentWithHeader((void*)0x352, 0x0, 0x2);
    EXPECT_EQ(adjustment, 0u);

    adjustment = alignForwardAdjustmentWithHeader((void*)0x351, 0x4, 0x0);
    EXPECT_EQ(adjustment, 3u);
}

 
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}