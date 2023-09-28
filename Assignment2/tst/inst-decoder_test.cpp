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

TEST(InstructionDecoderTest, GetBShouldReturnCorrectValue) {
    InstructionDecoder decoder;

    // Test case 1: Value with B in bits 15-11 is 0x0000F800
    decoder.setInstructionWord(0x0000F800);
    EXPECT_EQ(decoder.getB(), 0x000F);

    // Test case 2: Value with B in bits 15-11 is 0x001F0000
    decoder.setInstructionWord(0x001F0000);
    EXPECT_EQ(decoder.getB(), 0x001F);

    // Add more test cases as needed
}

TEST(InstructionDecoderTest, GetDShouldReturnCorrectValue) {
    InstructionDecoder decoder;

    // Test case 1: Value with D in bits 25-21 is 0x03E00000
    decoder.setInstructionWord(0x03E00000);
    EXPECT_EQ(decoder.getD(), 0x007C);

    // Test case 2: Value with D in bits 25-21 is 0x001F0000
    decoder.setInstructionWord(0x001F0000);
    EXPECT_EQ(decoder.getD(), 0x000F);

    // Add more test cases as needed
}

TEST(InstructionDecoderTest, GetOpcodeShouldReturnCorrectValue) {
    InstructionDecoder decoder;

    // Test case 1: Value with opcode in bits 31-26 is 0xFC000000
    decoder.setInstructionWord(0xFC000000);
    EXPECT_EQ(decoder.getOpcode(), 0xFC00);

    // Test case 2: Value with opcode in bits 31-26 is 0x001F0000
    decoder.setInstructionWord(0x001F0000);
    EXPECT_EQ(decoder.getOpcode(), 0x0000);

    // Add more test cases as needed
}

TEST(InstructionDecoderTest, GetOp2ShouldReturnCorrectValue) {
    InstructionDecoder decoder;

    // Test case 1: Value with op2 in bits 9-8 is 0x00000300
    decoder.setInstructionWord(0x00000300);
    EXPECT_EQ(decoder.getOp2(), 0x03);

    // Test case 2: Value with op2 in bits 7-6 is 0x000000C0
    decoder.setInstructionWord(0x000000C0);
    EXPECT_EQ(decoder.getOp2(), 0x03);

    // Add more test cases as needed
}

TEST(InstructionDecoderTest, GetOp3ShouldReturnCorrectValue) {
    InstructionDecoder decoder;

    // Test case 1: Value with op3 in bits 3-0 is 0x0000000F
    decoder.setInstructionWord(0x0000000F);
    EXPECT_EQ(decoder.getOp3(), 0x0F);

    // Test case 2: Value with op3 in bits 3-0 is 0x00000000
    decoder.setInstructionWord(0x00000000);
    EXPECT_EQ(decoder.getOp3(), 0x00);

    // Add more test cases as needed
}

TEST(InstructionDecoderTest, GetImmediateIShouldReturnCorrectValue) {
    InstructionDecoder decoder;

    // Test case 1: Value with immediate I in bits 15-0 is 0x0000FFFF
    decoder.setInstructionWord(0x0000FFFF);
    EXPECT_EQ(decoder.getImmediateI(), 0xFFFF);

    // Test case 2: Value with immediate I in bits 15-0 is 0x001F0000
    decoder.setInstructionWord(0x001F0000);
    EXPECT_EQ(decoder.getImmediateI(), 0x0000);

    // Add more test cases as needed
}

TEST(InstructionDecoderTest, GetImmediateNShouldReturnCorrectValue) {
    InstructionDecoder decoder;

    // Test case 1: Value with immediate N in bits 25-0 is 0x03FFFFFF
    decoder.setInstructionWord(0x03FFFFFF);
    EXPECT_EQ(decoder.getImmediateN(), 0x03FFFFFF);

    // Test case 2: Value with immediate N in bits 25-0 is 0x00000000
    decoder.setInstructionWord(0x00000000);
    EXPECT_EQ(decoder.getImmediateN(), 0x00000000);

    // Add more test cases as needed
}

TEST(InstructionDecoderTest, GetReservedShouldReturnCorrectValue) {
    InstructionDecoder decoder;

    // Test case 1: Value with reserved bits set is 0x00000FF0
    decoder.setInstructionWord(0x00000FF0);
    EXPECT_EQ(decoder.getReserved(), 0x00F);

    // Test case 2: Value with reserved bits set is 0x001F0000
    decoder.setInstructionWord(0x001F0000);
    EXPECT_EQ(decoder.getReserved(), 0x00);

    // Add more test cases as needed
}

TEST(InstructionDecoderTest, GetLShouldReturnCorrectValue) {
    InstructionDecoder decoder;

    // Test case 1: Value with L in bits 5-0 is 0x0000003F
    decoder.setInstructionWord(0x0000003F);
    EXPECT_EQ(decoder.getL(), 0x3F);

    // Test case 2: Value with L in bits 5-0 is 0x001F0000
    decoder.setInstructionWord(0x001F0000);
    EXPECT_EQ(decoder.getL(), 0x00);

    // Add more test cases as needed
}
