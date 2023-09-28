// getA_test.cpp

#include <gtest/gtest.h>
#include "inst-decoder.h"
// Assuming the relevant headers are included here

TEST(InstructionDecoderTest, GetAShouldReturnCorrectValue) {
    InstructionDecoder decoder;

    // Test case 1: Value with A in bits 20-16 is 0x00160000
    decoder.setInstructionWord(0x00160000);
    EXPECT_EQ(decoder.getA(), 0x0016);

    // Test case 2: Value with A in bits 20-16 is 0x001F0000
    decoder.setInstructionWord(0x001F0000);
    EXPECT_EQ(decoder.getA(), 0x001F);

    // Add more test cases as needed
}