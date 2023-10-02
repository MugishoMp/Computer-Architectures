// getA_test.cpp

#include <gtest/gtest.h>
#include "inst-decoder.h"
// Assuming the relevant headers are included here

TEST(InstructionDecoderTest, GetInstructionWordShouldReturnCorrectValue) {
    InstructionDecoder decoder;

    decoder.setInstructionWord(0x89ABCDEF);
    EXPECT_EQ(decoder.getInstructionWord(), 0x89ABCDEF);

    decoder.setInstructionWord(0xBBA5F32E);
    EXPECT_EQ(decoder.getInstructionWord(), 0xBBA5F32E);

    // Add more test cases as needed
}

TEST(InstructionDecoderTest, GetInstructionCodeShouldReturnCorrectType) {
    InstructionDecoder decoder;
    // Add a test case for each instruction type
    // 00 - J
    // Test case 1: Instruction that is of instruction type J
    decoder.setInstructionWord(0x00000000);
    EXPECT_EQ(decoder.getInstructionType(), (InstructionType) J);
    // 01 - J
    // 02 - .
    // 03 - J
    // 04 - J
    // 05 - .
    // 06 - .
    // 08 - .
    // 09 - .
    // 0A - . 
    // 11 - .
    // 12 - .
    // 13 - .
    // 1A - I
    // Test case 15: Instruction that is of instruction type I
    decoder.setInstructionWord(0x1A000000);
    EXPECT_EQ(decoder.getInstructionType(), (InstructionType) I);
    // 1B - I
    // 1C - .
    // 1D - .
    // 1E - .
    // 1F - .
    // 20 - I
    // 21 - I
    // 22 - I 
    // 23 - I
    // 24 - I
    // 25 - I
    // 26 - I
    // 27 - I
    // 28 - I
    // 29 - .
    // 2A - .
    // 2B - I
    // 2C - I
    // 2D - .
    // 2E - SH
    // Test case 36: Instruction that is of instruction type SH
    decoder.setInstructionWord(0x2E000000);
    EXPECT_EQ(decoder.getInstructionType(), (InstructionType) SH);
    // 2F - F
    // Test case 37: Instruction that is of instruction type F
    decoder.setInstructionWord(0x2F000000);
    EXPECT_EQ(decoder.getInstructionType(), (InstructionType) F);
    // 30 - .
    // 31 - .
    // 32 - .
    // 33 - S
    // Test case 41: Instruction that is of instruction type S
    decoder.setInstructionWord(0x33000000);
    EXPECT_EQ(decoder.getInstructionType(), (InstructionType) S);
    // 35 - S
    // 36 - S
    // 37 - S
    // 38 - R 
    // Test case 45: Instruction that is of instruction type R
    decoder.setInstructionWord(0x38000000);
    EXPECT_EQ(decoder.getInstructionType(), (InstructionType) R);
    // 39 - .
    // 3C - .
    // 3D - .
    // 3E - .
    // 3F - .

}

TEST(InstructionDecoderTest, GetAShouldReturnCorrectValue) {
    InstructionDecoder decoder;

    // Test for R-type instruction (opcode 0x38)
    decoder.setInstructionWord(0x38190000); // Set opcode 0x38 and register A bits to 
    EXPECT_EQ(decoder.getA(), 0x19);

    // Test for I-type instruction (opcode 0x1A)
    decoder.setInstructionWord(0x1A190000); // Set opcode 0x1A and register A bits to 
    EXPECT_EQ(decoder.getA(), 0x19);

    // Test for S-type instruction (opcode 0x33)
    decoder.setInstructionWord(0x33190000); // Set opcode 0x33 and register A bits to 
    EXPECT_EQ(decoder.getA(), 0x19);

    // Test for SH-type instruction (opcode 0x2A)
    decoder.setInstructionWord(0x2A190000); // Set opcode 0x2A and register A bits to 
    EXPECT_EQ(decoder.getA(), 0x19);

    // Test for F-type instruction (opcode 0x2F)
    decoder.setInstructionWord(0x2F190000); // Set opcode 0x2F and register A bits to 
    EXPECT_EQ(decoder.getA(), 0x19);
}

TEST(InstructionDecoderTest, GetBShouldReturnCorrectValue) {
    InstructionDecoder decoder;

    // Test for R-type instruction (opcode 0x38)
    decoder.setInstructionWord(0x38008A00); // Set opcode 0x38 and register A bits to 
    EXPECT_EQ(decoder.getB(), 0x15);

    // Test for S-type instruction (opcode 0x33)
    decoder.setInstructionWord(0x3300A800); // Set opcode 0x33 and register A bits to 
    EXPECT_EQ(decoder.getB(), 0x15);

    // Add more test cases as needed
}

TEST(InstructionDecoderTest, GetDShouldReturnCorrectValue) {
    InstructionDecoder decoder;

    // Test for R-type instruction (opcode 0x38)
    decoder.setInstructionWord(0x38000000); // Set opcode 0x38 and register A bits to 
    EXPECT_EQ(decoder.getD(), 0x09);

    // Test for I-type instruction (opcode 0x1A)
    decoder.setInstructionWord(0x1A000000); // Set opcode 0x1A and register A bits to 
    EXPECT_EQ(decoder.getD(), 0x09);

    // Test for SH-type instruction (opcode 0x2A)
    decoder.setInstructionWord(0x2A000000); // Set opcode 0x2A and register A bits to 
    EXPECT_EQ(decoder.getD(), 0x09);

    // Add more test cases as needed
}

TEST(InstructionDecoderTest, GetOpcodeShouldReturnCorrectValue) {
    InstructionDecoder decoder;

    // Test case 1: Value with opcode in bits 31-26 is 0xFC000000
    decoder.setInstructionWord(0xFC000000);
    EXPECT_EQ(decoder.getOpcode(), 0xFC);

    // Test case 2: Value with opcode in bits 31-26 is 0x001F0000
    decoder.setInstructionWord(0x039ABCDE);
    EXPECT_EQ(decoder.getOpcode(), 0x00);

    // Add more test cases as needed
}

TEST(InstructionDecoderTest, GetOp2ShouldReturnCorrectValue) {
    InstructionDecoder decoder;

    // Test for R-type instruction (opcode 0x38)
    // Retrieve from bits 9-8
    decoder.setInstructionWord(0x38000000); // Set opcode 0x38 and register A bits to 
    EXPECT_EQ(decoder.getOp2(), 0x00);

    // Test for SH-type instruction (opcode 0x2A)
    // Retrieve from bits 7-6
    decoder.setInstructionWord(0x2A000000); // Set opcode 0x2A and register A bits to 
    EXPECT_EQ(decoder.getOp2(), 0x00);

    // Add more test cases as needed
}

TEST(InstructionDecoderTest, GetOp3ShouldReturnCorrectValue) {
    InstructionDecoder decoder;

    // Test for R-type instruction (opcode 0x38)
    // Retrieve from bits 9-8
    decoder.setInstructionWord(0x38000000); // Set opcode 0x38 and register A bits to 
    EXPECT_EQ(decoder.getOp3(), 0x00);

    // Add more test cases as needed
}

TEST(InstructionDecoderTest, GetImmediateIShouldReturnCorrectValue) {
    InstructionDecoder decoder;

    // Test for I-type instruction (opcode 0x1A)
    // Retrieve from bits 15-0
    decoder.setInstructionWord(0x1A000000); // Set opcode 0x1A and register A bits to 
    EXPECT_EQ(decoder.getImmediateI(), 0x00);

    // Test for S-type instruction (opcode 0x33)
    // Retrieve from bits 15-0
    decoder.setInstructionWord(0x33000000); // Set opcode 0x33 and register A bits to 
    EXPECT_EQ(decoder.getImmediateI(), 0x00);

    // Test for F-type instruction (opcode 0x2F)
    // Retrieve from bits 10-0
    decoder.setInstructionWord(0x2F000000); // Set opcode 0x2F and register A bits to 
    EXPECT_EQ(decoder.getImmediateI(), 0x00);

    // Add more test cases as needed
}

TEST(InstructionDecoderTest, GetImmediateNShouldReturnCorrectValue) {
    InstructionDecoder decoder;

    // Test for J-type instruction (opcode 0x00)
    decoder.setInstructionWord(0x00000000); // Set opcode 0x2F and register A bits to 
    EXPECT_EQ(decoder.getImmediateN(), 0x00);

    // Add more test cases as needed
}

TEST(InstructionDecoderTest, GetReservedShouldReturnCorrectValue) {
    InstructionDecoder decoder;

    // Test for R-type instruction (opcode 0x38)
    decoder.setInstructionWord(0x38000000); // Set opcode 0x38 and register A bits to 
    EXPECT_EQ(decoder.getReserved(), 0x00);

    // Test for S-type instruction (opcode 0x33)
    decoder.setInstructionWord(0x33000000); // Set opcode 0x33 and register A bits to 
    EXPECT_EQ(decoder.getReserved(), 0x00);

    // Add more test cases as needed
}

TEST(InstructionDecoderTest, GetLShouldReturnCorrectValue) {
    InstructionDecoder decoder;

    // Test for SH-type instruction (opcode 0x2A)
    decoder.setInstructionWord(0x2A000000); // Set opcode 0x2A and register A bits to 
    EXPECT_EQ(decoder.getL(), 0x00);

    // Add more test cases as needed
}
