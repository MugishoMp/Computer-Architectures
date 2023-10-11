
#include <gtest/gtest.h>
#include "inst-decoder.h"  // Assuming the relevant headers are included here

// tests both getter and setter
TEST(InstructionDecoderTest, GetInstructionWordShouldReturnCorrectValue) {
    InstructionDecoder decoder;
    decoder.setInstructionWord(0x89ABCDEF);
    EXPECT_EQ(decoder.getInstructionWord(), 0x89ABCDEF);

    decoder.setInstructionWord(0xBBA5F32E);
    EXPECT_EQ(decoder.getInstructionWord(), 0xBBA5F32E);

    decoder.setInstructionWord(0x00000000);
    EXPECT_EQ(decoder.getInstructionWord(), 0x00000000);
}

TEST(InstructionDecoderTest, GetInstructionTypeShouldReturnCorrectValue) {
    InstructionDecoder decoder;

    // For J Type
    uint8_t j_opcodes[] = {0x00, 0x01, 0x03, 0x04};
    for (auto op : j_opcodes) {
        decoder.setInstructionWord(op << 26);
        EXPECT_EQ(decoder.getInstructionType(), J);
    }

    // For DN Type
    decoder.setInstructionWord(0x02 << 26);
    EXPECT_EQ(decoder.getInstructionType(), DN);

    // For ORK Type
    decoder.setInstructionWord(0x05 << 26);
    EXPECT_EQ(decoder.getInstructionType(), ORK);

    // For DROK Type
    decoder.setInstructionWord(0x06 << 26);
    EXPECT_EQ(decoder.getInstructionType(), DROK);

    // For OK Type
    decoder.setInstructionWord(0x08 << 26);
    EXPECT_EQ(decoder.getInstructionType(), OK);

    // For RES Type
    uint8_t res_opcodes[] = {0x09, 0x1C, 0x1D, 0x1E, 0x1F, 0x3D, 0x3E, 0x3F};
    for (auto op : res_opcodes) {
        decoder.setInstructionWord(op << 26);
        EXPECT_EQ(decoder.getInstructionType(), RES);
    }

    // For DABROO Type
    uint8_t dabroo_opcodes[] = {0x0A, 0x32};
    for (auto op : dabroo_opcodes) {
        decoder.setInstructionWord(op << 26);
        EXPECT_EQ(decoder.getInstructionType(), DABROO);
    }

    // For RBR Type
    uint8_t rbr_opcodes[] = {0x11, 0x12};
    for (auto op : rbr_opcodes) {
        decoder.setInstructionWord(op << 26);
        EXPECT_EQ(decoder.getInstructionType(), RBR);
    }

    // For RAI Type
    decoder.setInstructionWord(0x13 << 26);
    EXPECT_EQ(decoder.getInstructionType(), RAI);

    // For I Type
    uint8_t i_opcodes[] = {0x1A, 0x1B, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x2B, 0x2C};
    for (auto op : i_opcodes) {
        decoder.setInstructionWord(op << 26);
        EXPECT_EQ(decoder.getInstructionType(), I);
    }

    // For DAK Type
    uint8_t dak_opcodes[] = {0x29, 0x2A, 0x2D};
    for (auto op : dak_opcodes) {
        decoder.setInstructionWord(op << 26);
        EXPECT_EQ(decoder.getInstructionType(), DAK);
    }

    // For SH Type
    decoder.setInstructionWord(0x2E << 26);
    EXPECT_EQ(decoder.getInstructionType(), SH);

    // For F Type
    decoder.setInstructionWord(0x2F << 26);
    EXPECT_EQ(decoder.getInstructionType(), F);

    // For KABK Type
    decoder.setInstructionWord(0x30 << 26);
    EXPECT_EQ(decoder.getInstructionType(), KABK);

    // For RABRO Type
    decoder.setInstructionWord(0x31 << 26);
    EXPECT_EQ(decoder.getInstructionType(), RABRO);

    // For S Type
    uint8_t s_opcodes[] = {0x33, 0x35, 0x36, 0x37};
    for (auto op : s_opcodes) {
        decoder.setInstructionWord(op << 26);
        EXPECT_EQ(decoder.getInstructionType(), S);
    }

    // For R Type
    decoder.setInstructionWord(0x38 << 26);
    EXPECT_EQ(decoder.getInstructionType(), R);

    // For OABR Type
    decoder.setInstructionWord(0x39 << 26);
    EXPECT_EQ(decoder.getInstructionType(), OABR);

    // For DABLK Type
    decoder.setInstructionWord(0x3C << 26);
    EXPECT_EQ(decoder.getInstructionType(), DABLK);

    // For INVALID Type
    // Assuming that 0x3B is not a valid opcode
    decoder.setInstructionWord(0x3B << 26);
    EXPECT_EQ(decoder.getInstructionType(), INVALID);
}
// Add unit tests for getOpcode() function
TEST(InstructionDecoderTest, GetOpcodeShouldReturnCorrectValue) {
    InstructionDecoder decoder;

    // Test Case 1: For F type, opcode should be extracted from bits 31 to 21
    // 0010 1111
    // 1011 1100
    //      0011 1110
    // 101 1111 1111
    // 5FF
    uint32_t instructionWord = ((0x2F << 26) | 0x03E00000);
    decoder.setInstructionWord(instructionWord);
    EXPECT_EQ(decoder.getOpcode(), 0x5FF);

    // Test Case 2: For default types, opcode should be extracted from bits 31 to 26
    
    // Test Case 2: For default types, opcode should be extracted from bits 31 to 26
    uint8_t opcodes[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x08, 
        0x09, 0x0A, 0x11, 0x12, 0x13, 0x1A, 0x1B, 0x1C, 
        0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 
        0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 
        0x2D, 0x2E, 0x30, 0x31, 0x32, 0x33, 0x35, 0x36, 
        0x37, 0x38, 0x39, 0x3C, 0x3D, 0x3E, 0x3F};
    for (auto op : opcodes) {
        uint32_t instructionWord = (op << 26);
        decoder.setInstructionWord(instructionWord);
        EXPECT_EQ(decoder.getOpcode(), op);
    }

}

// Add unit tests for getOp2() function
TEST(InstructionDecoderTest, GetOp2ShouldReturnCorrectValue) {
    InstructionDecoder decoder;

    // For R Type
    decoder.setInstructionWord((0x38 << 26) | BITS_9_8);
    EXPECT_EQ(decoder.getOp2(), 0b11);

    // For SH Type
    decoder.setInstructionWord((0x2E << 26) |  BITS_7_6);
    EXPECT_EQ(decoder.getOp2(), 0b11);

    // For ORK Type
    decoder.setInstructionWord((0x05 << 26) | BITS_25_24);
    EXPECT_EQ(decoder.getOp2(), 0b11);

    // For DROK Type
    decoder.setInstructionWord((0x06 << 26) | BITS_16);
    EXPECT_EQ(decoder.getOp2(), 0b1);

    // For OK Type
    decoder.setInstructionWord((0x08 << 26) | 0x01FF0000);
    EXPECT_EQ(decoder.getOp2(), 0x01FF);

    // For OK Type
    decoder.setInstructionWord((0x08 << 26) | 0x03FFFFFF);
    EXPECT_EQ(decoder.getOp2(), 0x03FFFFFF);

    // For DABROO Type
    uint8_t dabroo_opcodes[] = {0x0A, 0x32};
    for (auto op : dabroo_opcodes) {
        decoder.setInstructionWord((op << 26) | 0b11011111);
        EXPECT_EQ(decoder.getOp2(), 0b1101);
        decoder.setInstructionWord((op << 26) | 0b11010000);
        EXPECT_EQ(decoder.getOp2(), 0b1101);
        decoder.setInstructionWord((op << 26) | 0b11101111);
        EXPECT_EQ(decoder.getOp2(), 0b1110);
        decoder.setInstructionWord((op << 26) | 0b11100000);
        EXPECT_EQ(decoder.getOp2(), 0b1110);
        if (op == 0x0A) {
            decoder.setInstructionWord((op << 26) | 0b11001111);
            EXPECT_EQ(decoder.getOp2(), 0b1100);
            decoder.setInstructionWord((op << 26) | 0b11111111);
            EXPECT_EQ(decoder.getOp2(), 0b1111);
        }
        decoder.setInstructionWord((op << 26) | 0b01111111);
        EXPECT_EQ(decoder.getOp2(), 0b01111111);
    }

    // For RABRO Type
    decoder.setInstructionWord((0x31 << 26) | BITS_3_0);
    EXPECT_EQ(decoder.getOp2(), 0b1111);

    // For OABR Type
    decoder.setInstructionWord((0x39 << 26) | BITS_25_21);
    EXPECT_EQ(decoder.getOp2(), 0b11111);

    // For all the other types
}

// Add unit tests for getOp3() function
TEST(InstructionDecoderTest, GetOp3ShouldReturnCorrectValue) {
    InstructionDecoder decoder;

    // For R Type
    decoder.setInstructionWord((0x38 << 26) | BITS_3_0);
    EXPECT_EQ(decoder.getOp3(), 0b1111);

    // For the rest of the types
    uint8_t opcodes[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x08, 
        0x09, 0x0A, 0x11, 0x12, 0x13, 0x1A, 0x1B, 0x1C, 
        0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 
        0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 
        0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33, 0x35, 
        0x36, 0x37, 0x39, 0x3C, 0x3D, 0x3E, 0x3F};
    for (auto op : opcodes) {
        uint32_t instructionWord = (op << 26);
        decoder.setInstructionWord(instructionWord);
        EXPECT_EQ(decoder.getOp3(), FIELD_NOT_AVAILABLE_8_BIT);
    }

    // Add your test cases here
}

// // Add unit tests for getFunctionCode() function
// TEST(InstructionDecoderTest, GetFunctionCodeShouldReturnCorrectValue) {
//     InstructionDecoder decoder;

//     // Add your test cases here
// }

// Add unit tests for getA() function
TEST(InstructionDecoderTest, GetAShouldReturnCorrectValue) {
    InstructionDecoder decoder;

    ///////////////////// types that DO have the A field /////////////////////
    // For R Type
    decoder.setInstructionWord((0x38 << 26) | BITS_20_16);
    EXPECT_EQ(decoder.getA(), 0x1F);

    // For I Type
    uint8_t i_opcodes[] = {0x1A, 0x1B, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x2B, 0x2C};
    for (auto op : i_opcodes) {
        decoder.setInstructionWord((op << 26) | BITS_20_16);
        EXPECT_EQ(decoder.getA(), 0x1F);
    }

    // For S Type
    uint8_t s_opcodes[] = {0x33, 0x35, 0x36, 0x37};
    for (auto op : s_opcodes) {
        decoder.setInstructionWord((op << 26) | BITS_20_16);
        EXPECT_EQ(decoder.getA(), 0x1F);
    }

    // For SH Type
    decoder.setInstructionWord((0x2E << 26) | BITS_20_16);
    EXPECT_EQ(decoder.getA(), 0x1F);

    // For F Type
    decoder.setInstructionWord((0x2F << 26) | BITS_20_16);
    EXPECT_EQ(decoder.getA(), 0x1F);

    // For DABROO Type
    uint8_t dabroo_opcodes[] = {0x0A, 0x32};
    for (auto op : dabroo_opcodes) {
        decoder.setInstructionWord((op << 26) | BITS_20_16);
        EXPECT_EQ(decoder.getA(), 0x1F);
    }

    // For RAI Type
    decoder.setInstructionWord((0x13 << 26) | BITS_20_16);
    EXPECT_EQ(decoder.getA(), 0x1F);

    // For DAK Type
    uint8_t dak_opcodes[] = {0x29, 0x2A, 0x2D};
    for (auto op : dak_opcodes) {
        decoder.setInstructionWord((op << 26) | BITS_20_16);
        EXPECT_EQ(decoder.getA(), 0x1F);
    }

    // For KABK Type
    decoder.setInstructionWord((0x30 << 26) | BITS_20_16);
    EXPECT_EQ(decoder.getA(), 0x1F);

    // For RABRO Type
    decoder.setInstructionWord((0x31 << 26) | BITS_20_16);
    EXPECT_EQ(decoder.getA(), 0x1F);

    // For OABR Type
    decoder.setInstructionWord((0x39 << 26) | BITS_20_16);
    EXPECT_EQ(decoder.getA(), 0x1F);

    // For DABLK Type
    decoder.setInstructionWord((0x3C << 26) | BITS_20_16);
    EXPECT_EQ(decoder.getA(), 0x1F);


    ///////////////////// types that DO NOT have the A field /////////////////////
    // For J Type
    uint8_t j_opcodes[] = {0x00, 0x01, 0x03, 0x04};
    for (auto op : j_opcodes) {
        decoder.setInstructionWord(op << 26);
        EXPECT_EQ(decoder.getA(), FIELD_NOT_AVAILABLE_8_BIT);
    }

    // For DN Type
    decoder.setInstructionWord(0x02 << 26);
    EXPECT_EQ(decoder.getA(), FIELD_NOT_AVAILABLE_8_BIT);

    // For ORK Type
    decoder.setInstructionWord(0x05 << 26);
    EXPECT_EQ(decoder.getA(), FIELD_NOT_AVAILABLE_8_BIT);

    // For DROK Type
    decoder.setInstructionWord(0x06 << 26);
    EXPECT_EQ(decoder.getA(), FIELD_NOT_AVAILABLE_8_BIT);

    // For OK Type
    decoder.setInstructionWord(0x08 << 26);
    EXPECT_EQ(decoder.getA(), FIELD_NOT_AVAILABLE_8_BIT);

    // For RES Type
    uint8_t res_opcodes[] = {0x09, 0x1C, 0x1D, 0x1E, 0x1F, 0x3D, 0x3E, 0x3F};
    for (auto op : res_opcodes) {
        decoder.setInstructionWord(op << 26);
        EXPECT_EQ(decoder.getA(), FIELD_NOT_AVAILABLE_8_BIT);
    }

    // For RBR Type
    uint8_t rbr_opcodes[] = {0x11, 0x12};
    for (auto op : rbr_opcodes) {
        decoder.setInstructionWord(op << 26);
        EXPECT_EQ(decoder.getA(), FIELD_NOT_AVAILABLE_8_BIT);
    }

}

// Add unit tests for getB() function
TEST(InstructionDecoderTest, GetBShouldReturnCorrectValue) {
    InstructionDecoder decoder;

    ///////////////////// types that DO have the B field /////////////////////
    // For R Type
    decoder.setInstructionWord((0x38 << 26) | BITS_15_11);
    EXPECT_EQ(decoder.getB(), 0x1F);

    // For S Type
    uint8_t s_opcodes[] = {0x33, 0x35, 0x36, 0x37};
    for (auto op : s_opcodes) {
        decoder.setInstructionWord((op << 26) | BITS_15_11);
        EXPECT_EQ(decoder.getB(), 0x1F);
    }

    // For DABROO Type
    uint8_t dabroo_opcodes[] = {0x0A, 0x32};
    for (auto op : dabroo_opcodes) {
        decoder.setInstructionWord((op << 26) | BITS_15_11);
        EXPECT_EQ(decoder.getB(), 0x1F);
    }


    // For RBR Type
    uint8_t rbr_opcodes[] = {0x11, 0x12};
    for (auto op : rbr_opcodes) {
        decoder.setInstructionWord((op << 26) | BITS_15_11);
        EXPECT_EQ(decoder.getB(), 0x1F);
    }

    // For KABK Type
    decoder.setInstructionWord((0x30 << 26) | BITS_15_11);
    EXPECT_EQ(decoder.getB(), 0x1F);

    // For RABRO Type
    decoder.setInstructionWord((0x31 << 26) | BITS_15_11);
    EXPECT_EQ(decoder.getB(), 0x1F);

    // For OABR Type
    decoder.setInstructionWord((0x39 << 26) | BITS_15_11);
    EXPECT_EQ(decoder.getB(), 0x1F);

    // For DABLK Type
    decoder.setInstructionWord((0x3C << 26) | BITS_15_11);
    EXPECT_EQ(decoder.getB(), 0x1F);


    ///////////////////// types that DO NOT have the B field /////////////////////
    // For J Type
    uint8_t j_opcodes[] = {0x00, 0x01, 0x03, 0x04};
    for (auto op : j_opcodes) {
        decoder.setInstructionWord(op << 26);
        EXPECT_EQ(decoder.getB(), FIELD_NOT_AVAILABLE_8_BIT);
    }

    // For DN Type
    decoder.setInstructionWord(0x02 << 26);
    EXPECT_EQ(decoder.getB(), FIELD_NOT_AVAILABLE_8_BIT);

    // For ORK Type
    decoder.setInstructionWord(0x05 << 26);
    EXPECT_EQ(decoder.getB(), FIELD_NOT_AVAILABLE_8_BIT);

    // For DROK Type
    decoder.setInstructionWord(0x06 << 26);
    EXPECT_EQ(decoder.getB(), FIELD_NOT_AVAILABLE_8_BIT);

    // For OK Type
    decoder.setInstructionWord(0x08 << 26);
    EXPECT_EQ(decoder.getB(), FIELD_NOT_AVAILABLE_8_BIT);

    // For RES Type
    uint8_t res_opcodes[] = {0x09, 0x1C, 0x1D, 0x1E, 0x1F, 0x3D, 0x3E, 0x3F};
    for (auto op : res_opcodes) {
        decoder.setInstructionWord(op << 26);
        EXPECT_EQ(decoder.getB(), FIELD_NOT_AVAILABLE_8_BIT);
    }

    // For I Type
    uint8_t i_opcodes[] = {0x1A, 0x1B, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x2B, 0x2C};
    for (auto op : i_opcodes) {
        decoder.setInstructionWord((op << 26));
        EXPECT_EQ(decoder.getB(), FIELD_NOT_AVAILABLE_8_BIT);
    }

    // For SH Type
    decoder.setInstructionWord((0x2E << 26));
    EXPECT_EQ(decoder.getB(), FIELD_NOT_AVAILABLE_8_BIT);

    // For F Type
    decoder.setInstructionWord((0x2F << 26));
    EXPECT_EQ(decoder.getB(), FIELD_NOT_AVAILABLE_8_BIT);

    // For RAI Type
    decoder.setInstructionWord((0x13 << 26));
    EXPECT_EQ(decoder.getB(), FIELD_NOT_AVAILABLE_8_BIT);

    // For DAK Type
    uint8_t dak_opcodes[] = {0x29, 0x2A, 0x2D};
    for (auto op : dak_opcodes) {
        decoder.setInstructionWord((op << 26));
        EXPECT_EQ(decoder.getB(), FIELD_NOT_AVAILABLE_8_BIT);
    }
}

// Add unit tests for getD() function
TEST(InstructionDecoderTest, GetDShouldReturnCorrectValue) {
    InstructionDecoder decoder;

    ///////////////////// types that DO have the D field /////////////////////
    // For R Type
    decoder.setInstructionWord((0x38 << 26) | BITS_25_21);
    EXPECT_EQ(decoder.getD(), 0x1F);

    // For I Type
    uint8_t i_opcodes[] = {0x1A, 0x1B, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x2B, 0x2C};
    for (auto op : i_opcodes) {
        decoder.setInstructionWord((op << 26) | BITS_25_21);
        EXPECT_EQ(decoder.getD(), 0x1F);
    }

    // For SH Type
    decoder.setInstructionWord((0x2E << 26) | BITS_25_21);
    EXPECT_EQ(decoder.getD(), 0x1F);

    // For DN Type
    decoder.setInstructionWord((0x02 << 26) | BITS_25_21);
    EXPECT_EQ(decoder.getD(), 0x1F);
    
    // For DROK Type
    decoder.setInstructionWord((0x06 << 26) | BITS_25_21);
    EXPECT_EQ(decoder.getD(), 0x1F);

    // For DABROO Type
    uint8_t dabroo_opcodes[] = {0x0A, 0x32};
    for (auto op : dabroo_opcodes) {
        decoder.setInstructionWord((op << 26) | BITS_25_21);
        EXPECT_EQ(decoder.getD(), 0x1F);
    }

    // For DAK Type
    uint8_t dak_opcodes[] = {0x29, 0x2A, 0x2D};
    for (auto op : dak_opcodes) {
        decoder.setInstructionWord((op << 26) | BITS_25_21);
        EXPECT_EQ(decoder.getD(), 0x1F);
    }

    // For DABLK Type
    decoder.setInstructionWord((0x3C << 26) | BITS_25_21);
    EXPECT_EQ(decoder.getD(), 0x1F);


    ///////////////////// types that DO NOT have the D field /////////////////////
    // For J Type
    uint8_t j_opcodes[] = {0x00, 0x01, 0x03, 0x04};
    for (auto op : j_opcodes) {
        decoder.setInstructionWord(op << 26);
        EXPECT_EQ(decoder.getD(), FIELD_NOT_AVAILABLE_8_BIT);
    }

    // For ORK Type
    decoder.setInstructionWord(0x05 << 26);
    EXPECT_EQ(decoder.getD(), FIELD_NOT_AVAILABLE_8_BIT);

    // For OK Type
    decoder.setInstructionWord(0x08 << 26);
    EXPECT_EQ(decoder.getD(), FIELD_NOT_AVAILABLE_8_BIT);

    // For RES Type
    uint8_t res_opcodes[] = {0x09, 0x1C, 0x1D, 0x1E, 0x1F, 0x3D, 0x3E, 0x3F};
    for (auto op : res_opcodes) {
        decoder.setInstructionWord(op << 26);
        EXPECT_EQ(decoder.getD(), FIELD_NOT_AVAILABLE_8_BIT);
    }

    // For F Type
    decoder.setInstructionWord(0x2F << 26);
    EXPECT_EQ(decoder.getD(), FIELD_NOT_AVAILABLE_8_BIT);

    // For RAI Type
    decoder.setInstructionWord(0x13 << 26);
    EXPECT_EQ(decoder.getD(), FIELD_NOT_AVAILABLE_8_BIT);


    // For S Type
    uint8_t s_opcodes[] = {0x33, 0x35, 0x36, 0x37};
    for (auto op : s_opcodes) {
        decoder.setInstructionWord(op << 26);
        EXPECT_EQ(decoder.getD(), FIELD_NOT_AVAILABLE_8_BIT);
    }

    // For RBR Type
    uint8_t rbr_opcodes[] = {0x11, 0x12};
    for (auto op : rbr_opcodes) {
        decoder.setInstructionWord(op << 26);
        EXPECT_EQ(decoder.getD(), FIELD_NOT_AVAILABLE_8_BIT);
    }

    // For KABK Type
    decoder.setInstructionWord(0x30 << 26);
    EXPECT_EQ(decoder.getD(), FIELD_NOT_AVAILABLE_8_BIT);

    // For RABRO Type
    decoder.setInstructionWord(0x31 << 26);
    EXPECT_EQ(decoder.getD(), FIELD_NOT_AVAILABLE_8_BIT);

    // For OABR Type
    decoder.setInstructionWord(0x39 << 26);
    EXPECT_EQ(decoder.getD(), FIELD_NOT_AVAILABLE_8_BIT);

    // Add your test cases here
}

// Add unit tests for getImmediateI() function
TEST(InstructionDecoderTest, GetImmediateIShouldReturnCorrectValue) {
    InstructionDecoder decoder;

    ///////////////////// types that DO have the Immedate N field /////////////////////
    // For I Type
    uint8_t i_opcodes[] = {0x1A, 0x1B, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x2B, 0x2C};
    for (auto op : i_opcodes) {
        decoder.setInstructionWord((op << 26) | BITS_15_0);
        EXPECT_EQ(decoder.getImmediateI(), 0xFFFF);
    }

    // For F Type
    decoder.setInstructionWord((0x2F << 26) | BITS_15_0);
    EXPECT_EQ(decoder.getImmediateI(), 0xFFFF);

    // For RAI Type
    decoder.setInstructionWord((0x13 << 26) | BITS_15_0);
    EXPECT_EQ(decoder.getImmediateI(), 0xFFFF);

    // For S Type
    uint8_t s_opcodes[] = {0x33, 0x35, 0x36, 0x37};
    for (auto op : s_opcodes) {
        decoder.setInstructionWord((op << 26) | BITS_10_0);
        EXPECT_EQ(decoder.getImmediateI(), 0x7FF);
    }



    ///////////////////// types that DO NOT have the Immedate N field /////////////////////
    // For SH Type
    decoder.setInstructionWord(0x2E << 26);
    EXPECT_EQ(decoder.getImmediateI(), FIELD_NOT_AVAILABLE_16_BIT);

    // For DN Type
    decoder.setInstructionWord(0x02 << 26);
    EXPECT_EQ(decoder.getImmediateI(), FIELD_NOT_AVAILABLE_16_BIT);
    
    // For DROK Type
    decoder.setInstructionWord(0x06 << 26);
    EXPECT_EQ(decoder.getImmediateI(), FIELD_NOT_AVAILABLE_16_BIT);

    // For DABROO Type
    uint8_t dabroo_opcodes[] = {0x0A, 0x32};
    for (auto op : dabroo_opcodes) {
        decoder.setInstructionWord(op << 26);
        EXPECT_EQ(decoder.getImmediateI(), FIELD_NOT_AVAILABLE_16_BIT);
    }

    // For DAK Type
    uint8_t dak_opcodes[] = {0x29, 0x2A, 0x2D};
    for (auto op : dak_opcodes) {
        decoder.setInstructionWord(op << 26);
        EXPECT_EQ(decoder.getImmediateI(), FIELD_NOT_AVAILABLE_16_BIT);
    }

    // For DABLK Type
    decoder.setInstructionWord(0x3C << 26);
    EXPECT_EQ(decoder.getImmediateI(), FIELD_NOT_AVAILABLE_16_BIT);

    // For R Type
    decoder.setInstructionWord(0x38 << 26);
    EXPECT_EQ(decoder.getImmediateI(), FIELD_NOT_AVAILABLE_16_BIT);

    // For J Type
    uint8_t j_opcodes[] = {0x00, 0x01, 0x03, 0x04};
    for (auto op : j_opcodes) {
        decoder.setInstructionWord(op << 26);
        EXPECT_EQ(decoder.getImmediateI(), FIELD_NOT_AVAILABLE_16_BIT);
    }

    // For ORK Type
    decoder.setInstructionWord(0x05 << 26);
    EXPECT_EQ(decoder.getImmediateI(), FIELD_NOT_AVAILABLE_16_BIT);

    // For OK Type
    decoder.setInstructionWord(0x08 << 26);
    EXPECT_EQ(decoder.getImmediateI(), FIELD_NOT_AVAILABLE_16_BIT);

    // For RES Type
    uint8_t res_opcodes[] = {0x09, 0x1C, 0x1D, 0x1E, 0x1F, 0x3D, 0x3E, 0x3F};
    for (auto op : res_opcodes) {
        decoder.setInstructionWord(op << 26);
        EXPECT_EQ(decoder.getImmediateI(), FIELD_NOT_AVAILABLE_16_BIT);
    }

    // For RBR Type
    uint8_t rbr_opcodes[] = {0x11, 0x12};
    for (auto op : rbr_opcodes) {
        decoder.setInstructionWord(op << 26);
        EXPECT_EQ(decoder.getImmediateI(), FIELD_NOT_AVAILABLE_16_BIT);
    }

    // For KABK Type
    decoder.setInstructionWord(0x30 << 26);
    EXPECT_EQ(decoder.getImmediateI(), FIELD_NOT_AVAILABLE_16_BIT);

    // For RABRO Type
    decoder.setInstructionWord(0x31 << 26);
    EXPECT_EQ(decoder.getImmediateI(), FIELD_NOT_AVAILABLE_16_BIT);

    // For OABR Type
    decoder.setInstructionWord(0x39 << 26);
    EXPECT_EQ(decoder.getImmediateI(), FIELD_NOT_AVAILABLE_16_BIT);
}

// Add unit tests for getImmediateN() function
TEST(InstructionDecoderTest, GetImmediateNShouldReturnCorrectValue) {
    InstructionDecoder decoder;

    ///////////////////// types that DO have the Immedate N field /////////////////////
    // For J Type
    uint8_t j_opcodes[] = {0x00, 0x01, 0x03, 0x04};
    for (auto op : j_opcodes) {
        decoder.setInstructionWord((op << 26) | BITS_25_0);
        EXPECT_EQ(decoder.getImmediateN(), 0x3FFFFFF);
    }

    // For DN Type
    decoder.setInstructionWord((0x02 << 26) | BITS_20_0);
    EXPECT_EQ(decoder.getImmediateN(), 0x1FFFFF);


    ///////////////////// types that DO NOT have the Immedate N field /////////////////////
    
    // For I Type
    uint8_t i_opcodes[] = {0x1A, 0x1B, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x2B, 0x2C};
    for (auto op : i_opcodes) {
        decoder.setInstructionWord(op << 26);
        EXPECT_EQ(decoder.getImmediateN(), FIELD_NOT_AVAILABLE_32_BIT);
    }

    // For F Type
    decoder.setInstructionWord(0x2F << 26);
    EXPECT_EQ(decoder.getImmediateN(), FIELD_NOT_AVAILABLE_32_BIT);

    // For RAI Type
    decoder.setInstructionWord(0x13 << 26);
    EXPECT_EQ(decoder.getImmediateN(), FIELD_NOT_AVAILABLE_32_BIT);

    // For S Type
    uint8_t s_opcodes[] = {0x33, 0x35, 0x36, 0x37};
    for (auto op : s_opcodes) {
        decoder.setInstructionWord(op << 26);
        EXPECT_EQ(decoder.getImmediateN(), FIELD_NOT_AVAILABLE_32_BIT);
    }

    // For SH Type
    decoder.setInstructionWord(0x2E << 26);
    EXPECT_EQ(decoder.getImmediateN(), FIELD_NOT_AVAILABLE_32_BIT);
    
    // For DROK Type
    decoder.setInstructionWord(0x06 << 26);
    EXPECT_EQ(decoder.getImmediateN(), FIELD_NOT_AVAILABLE_32_BIT);

    // For DABROO Type
    uint8_t dabroo_opcodes[] = {0x0A, 0x32};
    for (auto op : dabroo_opcodes) {
        decoder.setInstructionWord(op << 26);
        EXPECT_EQ(decoder.getImmediateN(), FIELD_NOT_AVAILABLE_32_BIT);
    }

    // For DAK Type
    uint8_t dak_opcodes[] = {0x29, 0x2A, 0x2D};
    for (auto op : dak_opcodes) {
        decoder.setInstructionWord(op << 26);
        EXPECT_EQ(decoder.getImmediateN(), FIELD_NOT_AVAILABLE_32_BIT);
    }

    // For DABLK Type
    decoder.setInstructionWord(0x3C << 26);
    EXPECT_EQ(decoder.getImmediateN(), FIELD_NOT_AVAILABLE_32_BIT);

    // For R Type
    decoder.setInstructionWord(0x38 << 26);
    EXPECT_EQ(decoder.getImmediateN(), FIELD_NOT_AVAILABLE_32_BIT);


    // For ORK Type
    decoder.setInstructionWord(0x05 << 26);
    EXPECT_EQ(decoder.getImmediateN(), FIELD_NOT_AVAILABLE_32_BIT);

    // For OK Type
    decoder.setInstructionWord(0x08 << 26);
    EXPECT_EQ(decoder.getImmediateN(), FIELD_NOT_AVAILABLE_32_BIT);

    // For RES Type
    uint8_t res_opcodes[] = {0x09, 0x1C, 0x1D, 0x1E, 0x1F, 0x3D, 0x3E, 0x3F};
    for (auto op : res_opcodes) {
        decoder.setInstructionWord(op << 26);
        EXPECT_EQ(decoder.getImmediateN(), FIELD_NOT_AVAILABLE_32_BIT);
    }

    // For RBR Type
    uint8_t rbr_opcodes[] = {0x11, 0x12};
    for (auto op : rbr_opcodes) {
        decoder.setInstructionWord(op << 26);
        EXPECT_EQ(decoder.getImmediateN(), FIELD_NOT_AVAILABLE_32_BIT);
    }

    // For KABK Type
    decoder.setInstructionWord(0x30 << 26);
    EXPECT_EQ(decoder.getImmediateN(), FIELD_NOT_AVAILABLE_32_BIT);

    // For RABRO Type
    decoder.setInstructionWord(0x31 << 26);
    EXPECT_EQ(decoder.getImmediateN(), FIELD_NOT_AVAILABLE_32_BIT);

    // For OABR Type
    decoder.setInstructionWord(0x39 << 26);
    EXPECT_EQ(decoder.getImmediateN(), FIELD_NOT_AVAILABLE_32_BIT);
}

// Add unit tests for getReserved() function
TEST(InstructionDecoderTest, GetReservedShouldReturnCorrectValue) {
    InstructionDecoder decoder;
    ///////////////////// types that DO have the Reserved field /////////////////////

    // For R Type
    decoder.setInstructionWord((0x38 << 26) | BITS_10 | BITS_7_4);
    EXPECT_EQ(decoder.getReserved(), 0x1F);

    // For SH Type
    decoder.setInstructionWord((0x2E << 26) | BITS_15_8);
    EXPECT_EQ(decoder.getReserved(), 0xFF);

    // For ORK Type
    decoder.setInstructionWord((0x05 << 26)  | BITS_23_16);
    EXPECT_EQ(decoder.getReserved(), 0xFF);

    // For DROK Type
    decoder.setInstructionWord((0x06 << 26) | BITS_20_17);
    EXPECT_EQ(decoder.getReserved(), 0xF);

    // For RES Type
    uint8_t res_opcodes[] = {0x09, 0x1C, 0x1D, 0x1E, 0x1F, 0x3D, 0x3E, 0x3F};
    for (auto op : res_opcodes) {
        decoder.setInstructionWord((op << 26) | BITS_25_0);
        EXPECT_EQ(decoder.getReserved(), 0x3FFFFFF);
    }



    // For DABROO Type
    // 0x0A
    decoder.setInstructionWord((0x0A << 26) | 0b11000000 | BITS_25_8 | BITS_3_0);
    EXPECT_EQ(decoder.getReserved(), 0x3FFFFF);
    // and more . . .
    decoder.setInstructionWord((0x0A << 26) | 0b01000000 | BITS_10_8);
    EXPECT_EQ(decoder.getReserved(), 0x7);

    // For DABROO Type
    // 0x32
    decoder.setInstructionWord((0x32 << 26) | 0b00001000 | BITS_25_21 | BITS_10_8);
    EXPECT_EQ(decoder.getReserved(), 0xFF);
    decoder.setInstructionWord((0x32 << 26) | 0b00101000 | BITS_25_21 | BITS_10_8);
    EXPECT_EQ(decoder.getReserved(), 0xFF);
    // and more . . .

    decoder.setInstructionWord((0x32 << 26) | 0b00011000 | BITS_25_21 | BITS_10);
    EXPECT_EQ(decoder.getReserved(), 0x3F);
    decoder.setInstructionWord((0x32 << 26) | 0b00111000 | BITS_25_21 | BITS_10);
    EXPECT_EQ(decoder.getReserved(), 0x3F);
    // and more . . .

    decoder.setInstructionWord((0x32 << 26) | 0b00110100 | BITS_25_21 | BITS_9_8);
    EXPECT_EQ(decoder.getReserved(), 0x7F);

    decoder.setInstructionWord((0x32 << 26) | 0b00110101 | BITS_25_21 | BITS_10 | BITS_8);
    EXPECT_EQ(decoder.getReserved(), 0x7F);


    decoder.setInstructionWord((0x32 << 26) | 0b11010000 | BITS_25_21 | BITS_3_0);
    EXPECT_EQ(decoder.getReserved(), 0x1FF);
    decoder.setInstructionWord((0x32 << 26) | 0b11100000 | BITS_25_21 | BITS_3_0);
    EXPECT_EQ(decoder.getReserved(), 0x1FF);

    decoder.setInstructionWord((0x32 << 26) | 0b00000000 | BITS_10_8);
    EXPECT_EQ(decoder.getReserved(), 0x7);
    // and more . . .

    decoder.setInstructionWord((0x32 << 26) | 0b00010101 | BITS_8);
    EXPECT_EQ(decoder.getReserved(), 0x1);
    decoder.setInstructionWord((0x32 << 26) | 0b00010100 | BITS_8);
    EXPECT_EQ(decoder.getReserved(), 0x1);



    // For RBR Type
    uint8_t rbr_opcodes[] = {0x11, 0x12};
    for (auto op : rbr_opcodes) {
        decoder.setInstructionWord((op << 26) | BITS_25_16 | BITS_10_0);
        EXPECT_EQ(decoder.getReserved(), 0x1FFFFF);
    }

    // For RAI Type
    decoder.setInstructionWord((0x13 << 26) | BITS_25_21);
    EXPECT_EQ(decoder.getReserved(), 0x1F);

    // For RABRO Type
    decoder.setInstructionWord((0x31 << 26) | BITS_25_21 | BITS_10_4);
    EXPECT_EQ(decoder.getReserved(), 0xFFF);


    // For OABR Type
    decoder.setInstructionWord((0x39 << 26) | BITS_10_0);
    EXPECT_EQ(decoder.getReserved(), 0x7FF);


    ///////////////////// types that DO NOT have the Reserved field /////////////////////
    // For J Type
    uint8_t j_opcodes[] = {0x00, 0x01, 0x03, 0x04};
    for (auto op : j_opcodes) {
        decoder.setInstructionWord(op << 26);
        EXPECT_EQ(decoder.getReserved(), FIELD_NOT_AVAILABLE_32_BIT);
    }

    // For DN Type
    decoder.setInstructionWord(0x02 << 26);
    EXPECT_EQ(decoder.getReserved(), FIELD_NOT_AVAILABLE_32_BIT);
    
    // For I Type
    uint8_t i_opcodes[] = {0x1A, 0x1B, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x2B, 0x2C};
    for (auto op : i_opcodes) {
        decoder.setInstructionWord(op << 26);
        EXPECT_EQ(decoder.getReserved(), FIELD_NOT_AVAILABLE_32_BIT);
    }

    // For F Type
    decoder.setInstructionWord(0x2F << 26);
    EXPECT_EQ(decoder.getReserved(), FIELD_NOT_AVAILABLE_32_BIT);

    // For S Type
    uint8_t s_opcodes[] = {0x33, 0x35, 0x36, 0x37};
    for (auto op : s_opcodes) {
        decoder.setInstructionWord(op << 26);
        EXPECT_EQ(decoder.getReserved(), FIELD_NOT_AVAILABLE_32_BIT);
    }

    // For DAK Type
    uint8_t dak_opcodes[] = {0x29, 0x2A, 0x2D};
    for (auto op : dak_opcodes) {
        decoder.setInstructionWord(op << 26);
        EXPECT_EQ(decoder.getReserved(), FIELD_NOT_AVAILABLE_32_BIT);
    }

    // For DABLK Type
    decoder.setInstructionWord(0x3C << 26);
    EXPECT_EQ(decoder.getReserved(), FIELD_NOT_AVAILABLE_32_BIT);

    // For OK Type
    decoder.setInstructionWord(0x08 << 26);
    EXPECT_EQ(decoder.getReserved(), FIELD_NOT_AVAILABLE_32_BIT);

    // For KABK Type
    decoder.setInstructionWord(0x30 << 26);
    EXPECT_EQ(decoder.getReserved(), FIELD_NOT_AVAILABLE_32_BIT);

}

// Add unit tests for getL() function
TEST(InstructionDecoderTest, GetLShouldReturnCorrectValue) {
    InstructionDecoder decoder;

    ///////////////////// types that DO have the L field /////////////////////
    // For SH Type
    decoder.setInstructionWord((0x2E << 26) | BITS_5_0);
    EXPECT_EQ(decoder.getL(), 0x3F);

    // For DABLK Type
    decoder.setInstructionWord((0x3C << 26) | BITS_10_5);
    EXPECT_EQ(decoder.getL(), 0x3F);

    ///////////////////// types that DO NOT have the L field /////////////////////
    // For J Type
    uint8_t j_opcodes[] = {0x00, 0x01, 0x03, 0x04};
    for (auto op : j_opcodes) {
        decoder.setInstructionWord((op << 26) | BITS_25_0);
        EXPECT_EQ(decoder.getL(), FIELD_NOT_AVAILABLE_8_BIT);
    }

    // For DN Type
    decoder.setInstructionWord((0x02 << 26) | BITS_20_0);
    EXPECT_EQ(decoder.getL(), FIELD_NOT_AVAILABLE_8_BIT);

    // For I Type
    uint8_t i_opcodes[] = {0x1A, 0x1B, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x2B, 0x2C};
    for (auto op : i_opcodes) {
        decoder.setInstructionWord(op << 26);
        EXPECT_EQ(decoder.getL(), FIELD_NOT_AVAILABLE_8_BIT);
    }

    // For F Type
    decoder.setInstructionWord(0x2F << 26);
    EXPECT_EQ(decoder.getL(), FIELD_NOT_AVAILABLE_8_BIT);

    // For RAI Type
    decoder.setInstructionWord(0x13 << 26);
    EXPECT_EQ(decoder.getL(), FIELD_NOT_AVAILABLE_8_BIT);

    // For S Type
    uint8_t s_opcodes[] = {0x33, 0x35, 0x36, 0x37};
    for (auto op : s_opcodes) {
        decoder.setInstructionWord(op << 26);
        EXPECT_EQ(decoder.getL(), FIELD_NOT_AVAILABLE_8_BIT);
    }

    
    // For DROK Type
    decoder.setInstructionWord(0x06 << 26);
    EXPECT_EQ(decoder.getL(), FIELD_NOT_AVAILABLE_8_BIT);

    // For DABROO Type
    uint8_t dabroo_opcodes[] = {0x0A, 0x32};
    for (auto op : dabroo_opcodes) {
        decoder.setInstructionWord(op << 26);
        EXPECT_EQ(decoder.getL(), FIELD_NOT_AVAILABLE_8_BIT);
    }

    // For DAK Type
    uint8_t dak_opcodes[] = {0x29, 0x2A, 0x2D};
    for (auto op : dak_opcodes) {
        decoder.setInstructionWord(op << 26);
        EXPECT_EQ(decoder.getL(), FIELD_NOT_AVAILABLE_8_BIT);
    }

    // For R Type
    decoder.setInstructionWord(0x38 << 26);
    EXPECT_EQ(decoder.getL(), FIELD_NOT_AVAILABLE_8_BIT);


    // For ORK Type
    decoder.setInstructionWord(0x05 << 26);
    EXPECT_EQ(decoder.getL(), FIELD_NOT_AVAILABLE_8_BIT);

    // For OK Type
    decoder.setInstructionWord(0x08 << 26);
    EXPECT_EQ(decoder.getL(), FIELD_NOT_AVAILABLE_8_BIT);

    // For RES Type
    uint8_t res_opcodes[] = {0x09, 0x1C, 0x1D, 0x1E, 0x1F, 0x3D, 0x3E, 0x3F};
    for (auto op : res_opcodes) {
        decoder.setInstructionWord(op << 26);
        EXPECT_EQ(decoder.getL(), FIELD_NOT_AVAILABLE_8_BIT);
    }

    // For RBR Type
    uint8_t rbr_opcodes[] = {0x11, 0x12};
    for (auto op : rbr_opcodes) {
        decoder.setInstructionWord(op << 26);
        EXPECT_EQ(decoder.getL(), FIELD_NOT_AVAILABLE_8_BIT);
    }

    // For KABK Type
    decoder.setInstructionWord(0x30 << 26);
    EXPECT_EQ(decoder.getL(), FIELD_NOT_AVAILABLE_8_BIT);

    // For RABRO Type
    decoder.setInstructionWord(0x31 << 26);
    EXPECT_EQ(decoder.getL(), FIELD_NOT_AVAILABLE_8_BIT);

    // For OABR Type
    decoder.setInstructionWord(0x39 << 26);
    EXPECT_EQ(decoder.getL(), FIELD_NOT_AVAILABLE_8_BIT);
}

// Add unit tests for getK() function
TEST(InstructionDecoderTest, GetKShouldReturnCorrectValue) {
    InstructionDecoder decoder;

    ///////////////////// types that DO have the K field /////////////////////
    // For DABLK Type
    decoder.setInstructionWord((0x3C << 26) | BITS_5_0);
    EXPECT_EQ(decoder.getK(), 0x3F);

    // For ORK Type
    decoder.setInstructionWord((0x05 << 26) | BITS_15_0);
    EXPECT_EQ(decoder.getK(), 0xFFFF);

    // For DROK Type
    decoder.setInstructionWord((0x06 << 26) | BITS_15_0);
    EXPECT_EQ(decoder.getK(), 0xFFFF);

    // For OK Type
    decoder.setInstructionWord((0x08 << 26) | BITS_15_0);
    EXPECT_EQ(decoder.getK(), 0xFFFF);

    // For DAK Type
    uint8_t dak_opcodes[] = {0x29, 0x2A, 0x2D};
    for (auto op : dak_opcodes) {
        decoder.setInstructionWord((op << 26) | BITS_15_0);
        EXPECT_EQ(decoder.getK(), 0xFFFF);
    }
    
    // For KABK Type
    decoder.setInstructionWord((0x30 << 26) | BITS_25_21 | BITS_10_0);
    EXPECT_EQ(decoder.getK(), 0xFFFF);



    ///////////////////// types that DO NOT have the K field /////////////////////
    // For SH Type
    decoder.setInstructionWord((0x2E << 26) | BITS_5_0);
    EXPECT_EQ(decoder.getK(), FIELD_NOT_AVAILABLE_32_BIT);


    // For J Type
    uint8_t j_opcodes[] = {0x00, 0x01, 0x03, 0x04};
    for (auto op : j_opcodes) {
        decoder.setInstructionWord((op << 26) | BITS_25_0);
        EXPECT_EQ(decoder.getK(), FIELD_NOT_AVAILABLE_32_BIT);
    }

    // For DN Type
    decoder.setInstructionWord((0x02 << 26) | BITS_20_0);
    EXPECT_EQ(decoder.getK(), FIELD_NOT_AVAILABLE_32_BIT);

    // For I Type
    uint8_t i_opcodes[] = {0x1A, 0x1B, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x2B, 0x2C};
    for (auto op : i_opcodes) {
        decoder.setInstructionWord(op << 26);
        EXPECT_EQ(decoder.getK(), FIELD_NOT_AVAILABLE_32_BIT);
    }

    // For F Type
    decoder.setInstructionWord(0x2F << 26);
    EXPECT_EQ(decoder.getK(), FIELD_NOT_AVAILABLE_32_BIT);

    // For RAI Type
    decoder.setInstructionWord(0x13 << 26);
    EXPECT_EQ(decoder.getK(), FIELD_NOT_AVAILABLE_32_BIT);

    // For S Type
    uint8_t s_opcodes[] = {0x33, 0x35, 0x36, 0x37};
    for (auto op : s_opcodes) {
        decoder.setInstructionWord(op << 26);
        EXPECT_EQ(decoder.getK(), FIELD_NOT_AVAILABLE_32_BIT);
    }

    // For DABROO Type
    uint8_t dabroo_opcodes[] = {0x0A, 0x32};
    for (auto op : dabroo_opcodes) {
        decoder.setInstructionWord(op << 26);
        EXPECT_EQ(decoder.getK(), FIELD_NOT_AVAILABLE_32_BIT);
    }

    // For R Type
    decoder.setInstructionWord(0x38 << 26);
    EXPECT_EQ(decoder.getK(), FIELD_NOT_AVAILABLE_32_BIT);

    // For RES Type
    uint8_t res_opcodes[] = {0x09, 0x1C, 0x1D, 0x1E, 0x1F, 0x3D, 0x3E, 0x3F};
    for (auto op : res_opcodes) {
        decoder.setInstructionWord(op << 26);
        EXPECT_EQ(decoder.getK(), FIELD_NOT_AVAILABLE_32_BIT);
    }

    // For RBR Type
    uint8_t rbr_opcodes[] = {0x11, 0x12};
    for (auto op : rbr_opcodes) {
        decoder.setInstructionWord(op << 26);
        EXPECT_EQ(decoder.getK(), FIELD_NOT_AVAILABLE_32_BIT);
    }

    // For RABRO Type
    decoder.setInstructionWord(0x31 << 26);
    EXPECT_EQ(decoder.getK(), FIELD_NOT_AVAILABLE_32_BIT);

    // For OABR Type
    decoder.setInstructionWord(0x39 << 26);
    EXPECT_EQ(decoder.getK(), FIELD_NOT_AVAILABLE_32_BIT);
}

// Add unit tests for getO() function
TEST(InstructionDecoderTest, GetOShouldReturnCorrectValue) {
    InstructionDecoder decoder;

    ///////////////////// types that DO have the L field /////////////////////
    // For DABROO Type
    decoder.setInstructionWord(0x32 << 26 | 0b00110100 | BITS_10);
    EXPECT_EQ(decoder.getO(), 0x1);
    decoder.setInstructionWord(0x32 << 26 | 0b00110101 | BITS_9);
    EXPECT_EQ(decoder.getO(), 0x1);
    
    decoder.setInstructionWord(0x32 << 26 | 0b00011000 | BITS_9_8);
    EXPECT_EQ(decoder.getO(), 0x3);
    decoder.setInstructionWord(0x32 << 26 | 0b111000 | BITS_9_8);
    EXPECT_EQ(decoder.getO(), 0x3);
    // and more . . .

    decoder.setInstructionWord(0x32 << 26 | 0b00010100 | BITS_10_9);
    EXPECT_EQ(decoder.getO(), 0x3);
    decoder.setInstructionWord(0x32 << 26 | 0b00010101 | BITS_10_9);
    EXPECT_EQ(decoder.getO(), 0x3);

    decoder.setInstructionWord(0x32 << 26 | 0b1101| BITS_10_8);
    EXPECT_EQ(decoder.getO(), 0x7);
    decoder.setInstructionWord(0x32 << 26 | 0b1110| BITS_10_8);
    EXPECT_EQ(decoder.getO(), 0x7);

    decoder.setInstructionWord(0x32 << 26 | 0b00010000 | BITS_10_8);
    EXPECT_EQ(decoder.getO(), 0x7);
    // and more . . .


    ///////////////////// types that DO NOT have the L field /////////////////////
    // For DABROO Type
    decoder.setInstructionWord(0x0A << 26);
    EXPECT_EQ(decoder.getO(), FIELD_NOT_AVAILABLE_8_BIT);

    // For SH Type
    decoder.setInstructionWord((0x2E << 26) | BITS_5_0);
    EXPECT_EQ(decoder.getO(), FIELD_NOT_AVAILABLE_8_BIT);

    // For DABLK Type
    decoder.setInstructionWord((0x3C << 26) | BITS_10_5);
    EXPECT_EQ(decoder.getO(), FIELD_NOT_AVAILABLE_8_BIT);

    // For J Type
    uint8_t j_opcodes[] = {0x00, 0x01, 0x03, 0x04};
    for (auto op : j_opcodes) {
        decoder.setInstructionWord((op << 26) | BITS_25_0);
        EXPECT_EQ(decoder.getO(), FIELD_NOT_AVAILABLE_8_BIT);
    }

    // For DN Type
    decoder.setInstructionWord((0x02 << 26) | BITS_20_0);
    EXPECT_EQ(decoder.getO(), FIELD_NOT_AVAILABLE_8_BIT);

    // For I Type
    uint8_t i_opcodes[] = {0x1A, 0x1B, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x2B, 0x2C};
    for (auto op : i_opcodes) {
        decoder.setInstructionWord(op << 26);
        EXPECT_EQ(decoder.getO(), FIELD_NOT_AVAILABLE_8_BIT);
    }

    // For F Type
    decoder.setInstructionWord(0x2F << 26);
    EXPECT_EQ(decoder.getO(), FIELD_NOT_AVAILABLE_8_BIT);

    // For RAI Type
    decoder.setInstructionWord(0x13 << 26);
    EXPECT_EQ(decoder.getO(), FIELD_NOT_AVAILABLE_8_BIT);

    // For S Type
    uint8_t s_opcodes[] = {0x33, 0x35, 0x36, 0x37};
    for (auto op : s_opcodes) {
        decoder.setInstructionWord(op << 26);
        EXPECT_EQ(decoder.getO(), FIELD_NOT_AVAILABLE_8_BIT);
    }

    
    // For DROK Type
    decoder.setInstructionWord(0x06 << 26);
    EXPECT_EQ(decoder.getO(), FIELD_NOT_AVAILABLE_8_BIT);

    // For DAK Type
    uint8_t dak_opcodes[] = {0x29, 0x2A, 0x2D};
    for (auto op : dak_opcodes) {
        decoder.setInstructionWord(op << 26);
        EXPECT_EQ(decoder.getO(), FIELD_NOT_AVAILABLE_8_BIT);
    }

    // For R Type
    decoder.setInstructionWord(0x38 << 26);
    EXPECT_EQ(decoder.getO(), FIELD_NOT_AVAILABLE_8_BIT);


    // For ORK Type
    decoder.setInstructionWord(0x05 << 26);
    EXPECT_EQ(decoder.getO(), FIELD_NOT_AVAILABLE_8_BIT);

    // For OK Type
    decoder.setInstructionWord(0x08 << 26);
    EXPECT_EQ(decoder.getO(), FIELD_NOT_AVAILABLE_8_BIT);

    // For RES Type
    uint8_t res_opcodes[] = {0x09, 0x1C, 0x1D, 0x1E, 0x1F, 0x3D, 0x3E, 0x3F};
    for (auto op : res_opcodes) {
        decoder.setInstructionWord(op << 26);
        EXPECT_EQ(decoder.getO(), FIELD_NOT_AVAILABLE_8_BIT);
    }

    // For RBR Type
    uint8_t rbr_opcodes[] = {0x11, 0x12};
    for (auto op : rbr_opcodes) {
        decoder.setInstructionWord(op << 26);
        EXPECT_EQ(decoder.getO(), FIELD_NOT_AVAILABLE_8_BIT);
    }

    // For KABK Type
    decoder.setInstructionWord(0x30 << 26);
    EXPECT_EQ(decoder.getO(), FIELD_NOT_AVAILABLE_8_BIT);

    // For RABRO Type
    decoder.setInstructionWord(0x31 << 26);
    EXPECT_EQ(decoder.getO(), FIELD_NOT_AVAILABLE_8_BIT);

    // For OABR Type
    decoder.setInstructionWord(0x39 << 26);
    EXPECT_EQ(decoder.getO(), FIELD_NOT_AVAILABLE_8_BIT);
}
