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

    decoder.setInstructionWord(0x00000000);
    EXPECT_EQ(decoder.getInstructionWord(), 0x00000000);

    // Add more test cases as needed
}

TEST(InstructionDecoderTest, GetInstructionCodeShouldReturnCorrectType) {
    InstructionDecoder decoder;
    // Add a test case for each instruction type
    // 00 - 0x00 - J
    // Test case 1: Instruction that is of instruction type J
    decoder.setInstructionWord(0x00000000);
    EXPECT_EQ(decoder.getInstructionType(), (InstructionType) J);
    // 01 - 0x04 - J
    // 02 - .
    // 03 - 0x0c - J
    // 04 - 0x10 - J
    // 05 - .
    // 06 - .
    // 08 - .
    // 09 - .
    // 0A - . 
    // 11 - .
    // 12 - .
    // 13 - .
    // 1A - 0x68 - I
    // Test case 15: Instruction that is of instruction type I
    decoder.setInstructionWord(0x68000000);
    EXPECT_EQ(decoder.getInstructionType(), (InstructionType) I);
    // 1B - 0x6C - I
    // 1C - .
    // 1D - .
    // 1E - .
    // 1F - .
    // 20 - 0x80 - I
    // 21 - 0x84 - I
    // 22 - 0x88 - I 
    // 23 - 0x8C - I
    // 24 - 0x90 - I
    // 25 - 0x94 - I
    // 26 - 0x98 - I
    // 27 - 0x9C - I
    // 28 - 0xA0 - I
    // 29 - .
    // 2A - .
    // 2B - 0xAC - I
    // 2C - 0xB0 - I
    // 2D - .
    // 2E - 0xB8 - SH
    // Test case 36: Instruction that is of instruction type SH
    decoder.setInstructionWord(0xB8000000);
    EXPECT_EQ(decoder.getInstructionType(), (InstructionType) SH);
    // 2F - 0xBC - F
    // Test case 37: Instruction that is of instruction type F
    decoder.setInstructionWord(0xBC000000);
    EXPECT_EQ(decoder.getInstructionType(), (InstructionType) F);
    // 30 - .
    // 31 - .
    // 32 - .
    // 33 - 0xCC - S
    // Test case 41: Instruction that is of instruction type S
    decoder.setInstructionWord(0xCC000000);
    EXPECT_EQ(decoder.getInstructionType(), (InstructionType) S);
    // 35 - 0xD4 - S
    // 36 - 0xD8 - S
    // 37 - 0xDC - S
    // 38 - 0xE0 - R 
    // Test case 45: Instruction that is of instruction type R
    decoder.setInstructionWord(0xE0000000);
    EXPECT_EQ(decoder.getInstructionType(), (InstructionType) R);
    // 39 - .
    // 3C - .
    // 3D - .
    // 3E - .
    // 3F - .

}

TEST(InstructionDecoderTest, GetOpcodeShouldReturnCorrectValue) {
    InstructionDecoder decoder;// Add a test case for each instruction type
    // 0x00 - 0x00 - J
    // Test case 1: Instruction that is of instruction type J
    decoder.setInstructionWord(0x00000000);
    EXPECT_EQ(decoder.getOpcode(), 0x00);
    // 0x01 - 0x04 - J
    // 0x02 - .
    // 0x03 - 0x0c - J
    // 0x04 - 0x10 - J
    // 0x05 - .
    // 0x06 - .
    // 0x08 - .
    // 0x09 - .
    // 0x0A - . 
    // 0x11 - .
    // 0x12 - .
    // 0x13 - .
    // 0x1A - 0x68 - I
    // Test case 15: Instruction that is of instruction type I
    decoder.setInstructionWord(0x68000000);
    EXPECT_EQ(decoder.getOpcode() , 0x1A);
    // 0x1B - 0x6C - I
    // 0x1C - .
    // 0x1D - .
    // 0x1E - .
    // 0x1F - .
    // 0x20 - 0x80 - I
    // 0x21 - 0x84 - I
    // 0x22 - 0x88 - I 
    // 0x23 - 0x8C - I
    // 0x24 - 0x90 - I
    // 0x25 - 0x94 - I
    // 0x26 - 0x98 - I
    // 0x27 - 0x9C - I
    // 0x28 - 0xA0 - I
    // 0x29 - .
    // 0x2A - .
    // 0x2B - 0xAC - I
    // 0x2C - 0xB0 - I
    // 0x2D - .
    // 0x2E - 0xB8 - SH
    // Test case 36: Instruction that is of instruction type SH
    decoder.setInstructionWord(0xB8000000);
    EXPECT_EQ(decoder.getOpcode(), 0x2E);
    // 0x2F - 0xBC - F
    // Test case 37: Instruction that is of instruction type F
    // returned opcode will be longer
    // instead of 6 bits it will be 11 bits long
    decoder.setInstructionWord(0xBC000000);
    EXPECT_EQ(decoder.getOpcode(), 0x5E0);
    // 0x30 - .
    // 0x31 - .
    // 0x32 - .
    // 0x33 - 0xCC - S
    // Test case 41: Instruction that is of instruction type S
    decoder.setInstructionWord(0xCC000000);
    EXPECT_EQ(decoder.getOpcode(), 0x33);
    // 0x35 - 0xD4 - S
    // 0x36 - 0xD8 - S
    // 0x37 - 0xDC - S
    // 0x38 - 0xE0 - R 
    // Test case 45: Instruction that is of instruction type R
    decoder.setInstructionWord(0xE0000000);
    EXPECT_EQ(decoder.getOpcode(), 0x38);
    // 0x39 - .
    // 0x3C - .
    // 0x3D - .
    // 0x3E - .
    // 0x3F - .
}

TEST(InstructionDecoderTest, GetAShouldReturnCorrectValue) {
    InstructionDecoder decoder;

    // Test for R-type instruction (opcode 0xE0)
    decoder.setInstructionWord(0xE01F0000); // Set opcode 0xE0 and register A bits to 
    EXPECT_EQ(decoder.getA(), 0x1F) << "R-type instruction";

    // Test for I-type instruction (opcode 0x68)
    decoder.setInstructionWord(0x681F0000); // Set opcode 0x68 and register A bits to 
    EXPECT_EQ(decoder.getA(), 0x1F) << "I-type instruction";

    // Test for S-type instruction (opcode 0xCC)
    decoder.setInstructionWord(0xCC1F0000); // Set opcode 0xCC and register A bits to 
    EXPECT_EQ(decoder.getA(), 0x1F) << "S-type instruction";

    // Test for SH-type instruction (opcode 0xB8)
    decoder.setInstructionWord(0xB81F0000); // Set opcode 0xB8 and register A bits to 
    EXPECT_EQ(decoder.getA(), 0x1F) << "SH-type instruction";

    // Test for F-type instruction (opcode 0xBC)
    decoder.setInstructionWord(0xBC1F0000); // Set opcode 0xBC and register A bits to 
    EXPECT_EQ(decoder.getA(), 0x1F) << "F-type instruction";

    // Test for J-type instruction (opcode 0x00)
    decoder.setInstructionWord(0x03FFFFFF); // Set opcode 0xBC and register A bits to 
    EXPECT_EQ(decoder.getA(), FIELD_NOT_AVAILABLE_8_BIT) << "J-type instruction";
}

TEST(InstructionDecoderTest, GetBShouldReturnCorrectValue) {
    InstructionDecoder decoder;

    // Test for R-type instruction (opcode 0xE0)
    decoder.setInstructionWord(0xE000F800); // Set opcode 0xE0 and register A bits to 
    EXPECT_EQ(decoder.getB(), 0x1F);

    // Test for I-type instruction (opcode 0x68)
    decoder.setInstructionWord(0x681F0000); // Set opcode 0x68 and register A bits to 
    EXPECT_EQ(decoder.getB(), FIELD_NOT_AVAILABLE_8_BIT);

    // Test for S-type instruction (opcode 0xCC)
    decoder.setInstructionWord(0xCC00F800); // Set opcode 0xCC and register A bits to 
    EXPECT_EQ(decoder.getB(), 0x1F);

    // Test for SH-type instruction (opcode 0xB8)
    decoder.setInstructionWord(0xB81F0000); // Set opcode 0xB8 and register A bits to 
    EXPECT_EQ(decoder.getB(), FIELD_NOT_AVAILABLE_8_BIT);

    // Test for F-type instruction (opcode 0xBC)
    decoder.setInstructionWord(0xBC1F0000); // Set opcode 0xBC and register A bits to 
    EXPECT_EQ(decoder.getB(), FIELD_NOT_AVAILABLE_8_BIT); 

    // Test for J-type instruction (opcode 0x00)
    decoder.setInstructionWord(0x03FFFFFF); // Set opcode 0xBC and register A bits to 
    EXPECT_EQ(decoder.getB(), FIELD_NOT_AVAILABLE_8_BIT);

    // Add more test cases as needed
}

TEST(InstructionDecoderTest, GetDShouldReturnCorrectValue) {
    InstructionDecoder decoder;

    // Test for R-type instruction (opcode 0xE0)
    decoder.setInstructionWord(0xE3E00000); // Set opcode 0xE0 and register A bits to 
    EXPECT_EQ(decoder.getD(), 0x1F);

    // Test for I-type instruction (opcode 0x68)
    decoder.setInstructionWord(0x6BE00000); // Set opcode 0x68 and register A bits to 
    EXPECT_EQ(decoder.getD(), 0x1F);

    // Test for S-type instruction (opcode 0xCC)
    decoder.setInstructionWord(0xCC00F800); // Set opcode 0xCC and register A bits to 
    EXPECT_EQ(decoder.getD(), FIELD_NOT_AVAILABLE_8_BIT);

    // Test for SH-type instruction (opcode 0xB8)
    decoder.setInstructionWord(0xBBE00000); // Set opcode 0xB8 and register A bits to 
    EXPECT_EQ(decoder.getD(), 0x1F);

    // Test for F-type instruction (opcode 0xBC)
    decoder.setInstructionWord(0xBC1F0000); // Set opcode 0xBC and register A bits to 
    EXPECT_EQ(decoder.getD(), FIELD_NOT_AVAILABLE_8_BIT); 

    // Test for J-type instruction (opcode 0x00)
    decoder.setInstructionWord(0x03FFFFFF); // Set opcode 0xBC and register A bits to 
    EXPECT_EQ(decoder.getD(), FIELD_NOT_AVAILABLE_8_BIT);

    // Add more test cases as needed
}

TEST(InstructionDecoderTest, GetOp2ShouldReturnCorrectValue) {
    InstructionDecoder decoder;

    // Test for R-type instruction (opcode 0xE0)
    // Retrieve from bits 9-8
    decoder.setInstructionWord(0xE0000300); // Set opcode 0xE0 and register A bits to 
    EXPECT_EQ(decoder.getOp2(), 0x3);

    // Test for I-type instruction (opcode 0x68)
    decoder.setInstructionWord(0x6BE00000); // Set opcode 0x68 and register A bits to 
    EXPECT_EQ(decoder.getOp2(), FIELD_NOT_AVAILABLE_8_BIT);

    // Test for S-type instruction (opcode 0xCC)
    decoder.setInstructionWord(0xCC00F800); // Set opcode 0xCC and register A bits to 
    EXPECT_EQ(decoder.getOp2(), FIELD_NOT_AVAILABLE_8_BIT);

    // Test for SH-type instruction (opcode 0xB8)
    // Retrieve from bits 7-6
    decoder.setInstructionWord(0xB80000C0); // Set opcode 0xB8 and register A bits to 
    EXPECT_EQ(decoder.getOp2(), 0x3);

    // Test for F-type instruction (opcode 0xBC)
    decoder.setInstructionWord(0xBC1F0000); // Set opcode 0xBC and register A bits to 
    EXPECT_EQ(decoder.getOp2(), FIELD_NOT_AVAILABLE_8_BIT); 

    // Test for J-type instruction (opcode 0x00)
    decoder.setInstructionWord(0x03FFFFFF); // Set opcode 0xBC and register A bits to 
    EXPECT_EQ(decoder.getOp2(), FIELD_NOT_AVAILABLE_8_BIT);

    // Add more test cases as needed
}

TEST(InstructionDecoderTest, GetOp3ShouldReturnCorrectValue) {
    InstructionDecoder decoder;

    // Test for R-type instruction (opcode 0xE0)
    // Retrieve from bits 3-0
    decoder.setInstructionWord(0xE000000F); // Set opcode 0xE0 and register A bits to 
    EXPECT_EQ(decoder.getOp3(), 0xF);

    // Test for I-type instruction (opcode 0x68)
    decoder.setInstructionWord(0x6BE00000); // Set opcode 0x68 and register A bits to 
    EXPECT_EQ(decoder.getOp3(), FIELD_NOT_AVAILABLE_8_BIT);

    // Test for S-type instruction (opcode 0xCC)
    decoder.setInstructionWord(0xCC00F800); // Set opcode 0xCC and register A bits to 
    EXPECT_EQ(decoder.getOp3(), FIELD_NOT_AVAILABLE_8_BIT);

    // Test for SH-type instruction (opcode 0xB8)
    // Retrieve from bits 7-6
    decoder.setInstructionWord(0xB80000C0); // Set opcode 0xB8 and register A bits to 
    EXPECT_EQ(decoder.getOp3(), FIELD_NOT_AVAILABLE_8_BIT);

    // Test for F-type instruction (opcode 0xBC)
    decoder.setInstructionWord(0xBC1F0000); // Set opcode 0xBC and register A bits to 
    EXPECT_EQ(decoder.getOp3(), FIELD_NOT_AVAILABLE_8_BIT); 

    // Test for J-type instruction (opcode 0x00)
    decoder.setInstructionWord(0x03FFFFFF); // Set opcode 0xBC and register A bits to 
    EXPECT_EQ(decoder.getOp3(), FIELD_NOT_AVAILABLE_8_BIT);

    // Add more test cases as needed
}

TEST(InstructionDecoderTest, GetFunctionCodeShouldReturnCorrectValue) {
    InstructionDecoder decoder;

    // Test for R-type instruction (opcode 0xE0)
    // Retrieve from bits 3-0
    decoder.setInstructionWord(0xE000030F); // Set opcode 0xE0 and register A bits to 
    EXPECT_EQ(decoder.getFunctionCode(), 0x3F);

    // Test for I-type instruction (opcode 0x68)
    decoder.setInstructionWord(0x6BE00000); // Set opcode 0x68 and register A bits to 
    EXPECT_EQ(decoder.getFunctionCode(), FIELD_NOT_AVAILABLE_8_BIT);

    // Test for S-type instruction (opcode 0xCC)
    decoder.setInstructionWord(0xCC00F800); // Set opcode 0xCC and register A bits to 
    EXPECT_EQ(decoder.getFunctionCode(), FIELD_NOT_AVAILABLE_8_BIT);

    // Test for SH-type instruction (opcode 0xB8)
    // Retrieve from bits 7-6
    decoder.setInstructionWord(0xB80000C0); // Set opcode 0xB8 and register A bits to 
    EXPECT_EQ(decoder.getFunctionCode(), 0x3);

    // Test for F-type instruction (opcode 0xBC)
    decoder.setInstructionWord(0xBC1F0000); // Set opcode 0xBC and register A bits to 
    EXPECT_EQ(decoder.getFunctionCode(), FIELD_NOT_AVAILABLE_8_BIT); 

    // Test for J-type instruction (opcode 0x00)
    decoder.setInstructionWord(0x03FFFFFF); // Set opcode 0xBC and register A bits to 
    EXPECT_EQ(decoder.getFunctionCode(), FIELD_NOT_AVAILABLE_8_BIT);

    // Add more test cases as needed
}

TEST(InstructionDecoderTest, GetImmediateIShouldReturnCorrectValue) {
    InstructionDecoder decoder;

    // Test for R-type instruction (opcode 0xE0)
    // Retrieve from bits 3-0
    decoder.setInstructionWord(0xE000000F); // Set opcode 0xE0 and register A bits to 
    EXPECT_EQ(decoder.getImmediateI(), FIELD_NOT_AVAILABLE_16_BIT);

    // Test for I-type instruction (opcode 0x68)
    // Retrieve from bits 15-0
    decoder.setInstructionWord(0x6800FFFF); // Set opcode 0x68 and register A bits to 
    EXPECT_EQ(decoder.getImmediateI(), 0xFFFF);

    // Test for S-type instruction (opcode 0xCC)
    // Retrieve from bits 15-0
    decoder.setInstructionWord(0xCC0007FF); // Set opcode 0xCC and register A bits to 
    EXPECT_EQ(decoder.getImmediateI(), 0x7FF);

    // Test for SH-type instruction (opcode 0xB8)
    // Retrieve from bits 7-6
    decoder.setInstructionWord(0xB80000C0); // Set opcode 0xB8 and register A bits to 
    EXPECT_EQ(decoder.getImmediateI(), FIELD_NOT_AVAILABLE_16_BIT);

    // Test for F-type instruction (opcode 0xBC)
    // Retrieve from bits 10-0
    decoder.setInstructionWord(0xBC00FFFF); // Set opcode 0xBC and register A bits to 
    EXPECT_EQ(decoder.getImmediateI(), 0xFFFF);

    // Test for J-type instruction (opcode 0x00)
    decoder.setInstructionWord(0x03FFFFFF); // Set opcode 0xBC and register A bits to 
    EXPECT_EQ(decoder.getImmediateI(), FIELD_NOT_AVAILABLE_16_BIT);

    // Add more test cases as needed
}

TEST(InstructionDecoderTest, GetImmediateNShouldReturnCorrectValue) {
    InstructionDecoder decoder;

    // Test for R-type instruction (opcode 0xE0)
    // Retrieve from bits 3-0
    decoder.setInstructionWord(0xE000000F); // Set opcode 0xE0 and register A bits to 
    EXPECT_EQ(decoder.getImmediateN(), FIELD_NOT_AVAILABLE_32_BIT);

    // Test for I-type instruction (opcode 0x68)
    // Retrieve from bits 15-0
    decoder.setInstructionWord(0x6800FFFF); // Set opcode 0x68 and register A bits to 
    EXPECT_EQ(decoder.getImmediateN(), FIELD_NOT_AVAILABLE_32_BIT);

    // Test for S-type instruction (opcode 0xCC)
    // Retrieve from bits 15-0
    decoder.setInstructionWord(0xCC00FFFF); // Set opcode 0xCC and register A bits to 
    EXPECT_EQ(decoder.getImmediateN(), FIELD_NOT_AVAILABLE_32_BIT);

    // Test for SH-type instruction (opcode 0xB8)
    // Retrieve from bits 7-6
    decoder.setInstructionWord(0xB80000C0); // Set opcode 0xB8 and register A bits to 
    EXPECT_EQ(decoder.getImmediateN(), FIELD_NOT_AVAILABLE_32_BIT);

    // Test for F-type instruction (opcode 0xBC)
    // Retrieve from bits 10-0
    decoder.setInstructionWord(0xBC0007FF); // Set opcode 0xBC and register A bits to 
    EXPECT_EQ(decoder.getImmediateN(), FIELD_NOT_AVAILABLE_32_BIT);

    // Test for J-type instruction (opcode 0x00)
    decoder.setInstructionWord(0x03FFFFFF); // Set opcode 0xBC and register A bits to 
    EXPECT_EQ(decoder.getImmediateN(), 0x3FFFFFF);

    // Add more test cases as needed
}

TEST(InstructionDecoderTest, GetReservedShouldReturnCorrectValue) {
    InstructionDecoder decoder;

    // Test for R-type instruction (opcode 0xE0)
    decoder.setInstructionWord(0xE00004F0); // Set opcode 0xE0 and register A bits to 
    EXPECT_EQ(decoder.getReserved(), 0x1F);

    // Test for I-type instruction (opcode 0x68)
    // Retrieve from bits 15-0
    decoder.setInstructionWord(0x6800FFFF); // Set opcode 0x68 and register A bits to 
    EXPECT_EQ(decoder.getReserved(), FIELD_NOT_AVAILABLE_8_BIT);

    // Test for S-type instruction (opcode 0xCC)
    decoder.setInstructionWord(0xCC0000C0); // Set opcode 0xCC and register A bits to 
    EXPECT_EQ(decoder.getReserved(), FIELD_NOT_AVAILABLE_8_BIT);

    // Test for SH-type instruction (opcode 0xB8)
    // Retrieve from bits 7-6
    decoder.setInstructionWord(0xB800FF00); // Set opcode 0xB8 and register A bits to 
    EXPECT_EQ(decoder.getReserved(), 0xFF);

    // Test for F-type instruction (opcode 0xBC)
    // Retrieve from bits 10-0
    decoder.setInstructionWord(0xBC0007FF); // Set opcode 0xBC and register A bits to 
    EXPECT_EQ(decoder.getReserved(), FIELD_NOT_AVAILABLE_8_BIT);

    // Test for J-type instruction (opcode 0x00)
    decoder.setInstructionWord(0x03FFFFFF); // Set opcode 0xBC and register A bits to 
    EXPECT_EQ(decoder.getReserved(), FIELD_NOT_AVAILABLE_8_BIT);


    // Add more test cases as needed
}

TEST(InstructionDecoderTest, GetLShouldReturnCorrectValue) {
    InstructionDecoder decoder;

    // Test for R-type instruction (opcode 0xE0)
    decoder.setInstructionWord(0xE00004F0); // Set opcode 0xE0 and register A bits to 
    EXPECT_EQ(decoder.getL(), FIELD_NOT_AVAILABLE_8_BIT);

    // Test for I-type instruction (opcode 0x68)
    // Retrieve from bits 15-0
    decoder.setInstructionWord(0x6800FFFF); // Set opcode 0x68 and register A bits to 
    EXPECT_EQ(decoder.getL(), FIELD_NOT_AVAILABLE_8_BIT);

    // Test for S-type instruction (opcode 0xCC)
    decoder.setInstructionWord(0xCC00FF00); // Set opcode 0xCC and register A bits to 
    EXPECT_EQ(decoder.getL(), FIELD_NOT_AVAILABLE_8_BIT);

    // Test for SH-type instruction (opcode 0xB8)
    decoder.setInstructionWord(0xB800003F); // Set opcode 0xB8 and register A bits to 
    EXPECT_EQ(decoder.getL(), 0x3F);

    // Test for F-type instruction (opcode 0xBC)
    // Retrieve from bits 10-0
    decoder.setInstructionWord(0xBC0007FF); // Set opcode 0xBC and register A bits to 
    EXPECT_EQ(decoder.getL(), FIELD_NOT_AVAILABLE_8_BIT);

    // Test for J-type instruction (opcode 0x00)
    decoder.setInstructionWord(0x03FFFFFF); // Set opcode 0xBC and register A bits to 
    EXPECT_EQ(decoder.getL(), FIELD_NOT_AVAILABLE_8_BIT);

    // Add more test cases as needed
}
