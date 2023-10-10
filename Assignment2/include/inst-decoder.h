/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    inst-decoder.h - RISC-V instruction decoder.
 *
 * Copyright (C) 2016,2019  Leiden University, The Netherlands.
 */

#ifndef __INST_DECODER_H__
#define __INST_DECODER_H__

#include "reg-file.h"

#include <stdexcept>
#include <cstdint>

static const int INSTRUCTION_SIZE = 4;

/* TODO: add enums and constants necessary for your instruction decoder. */

#define BITS_31_26 0xFC000000   // Represents 11111100 00000000 00000000 00000000
#define BITS_31_21 0xFFE00000   // Represents 11111111 11100000 00000000 00000000
#define BITS_25_21 0x03E00000   // Represents 00000011 11100000 00000000 00000000
#define BITS_25_16 0x03FF0000   // Represents 00000011 11111111 00000000 00000000
#define BITS_25_0  0x03FFFFFF   // Represents 00000011 11111111 11111111 11111111
#define BITS_23_16 0x00FF0000   // Represents 00000000 11111111 00000000 00000000
#define BITS_20_17 0x001E0000   // Represents 00000000 00011110 00000000 00000000
#define BITS_20_16 0x001F0000   // Represents 00000000 00011111 00000000 00000000
#define BITS_15_11 0x0000F800   // Represents 00000000 00000000 11111000 00000000
#define BITS_15_8  0x0000FF00   // Represents 00000000 00000000 11111111 00000000
#define BITS_15_0  0x0000FFFF   // Represents 00000000 00000000 11111111 11111111
#define BITS_10_0  0x000007FF   // Represents 00000000 00000000 00000111 11111111
#define BITS_10_4  0x000007F0   // Represents 00000000 00000000 00000111 11110000
#define BITS_10_5  0x000007E0   // Represents 00000000 00000000 00000111 11100000
#define BITS_10_8  0x00000700   // Represents 00000000 00000000 00000111 00000000
#define BITS_10    0x00000400   // Represents 00000000 00000000 00000100 00000000
#define BITS_9_8   0x00000300   // Represents 00000000 00000000 00000011 00000000
#define BITS_7_6   0x000000C0   // Represents 00000000 00000000 00000000 11000000
#define BITS_7_4   0x000000F0   // Represents 00000000 00000000 00000000 11110000
#define BITS_5_0   0x0000003F   // Represents 00000000 00000000 00000000 00111111
#define BITS_3_0   0x0000000F   // Represents 00000000 00000000 00000000 00001111
#define FIELD_NOT_AVAILABLE_8_BIT 255     // Value outside the range of valid values for 8-bit.
#define FIELD_NOT_AVAILABLE_16_BIT 65535  // Value outside the range of valid values for 16-bit.
#define FIELD_NOT_AVAILABLE_32_BIT 4294967295  // Value outside the range of valid values for 32-bit.

enum InstructionType {
  INVALID,
  R,
  I,
  S,
  SH,
  J,
  F,
  DN,
  ORK,
  DROK,
  OK,
  RES,
  DABROO,
  RBR,
  RAI,
  DAK,
  KABK,
  RABRO,
  OABR,
  DABLK
};

/* Exception that should be thrown when an illegal instruction
 * is encountered.
 */
class IllegalInstruction : public std::runtime_error
{
  public:
    explicit IllegalInstruction(const std::string &what)
      : std::runtime_error(what)
    { }

    explicit IllegalInstruction(const char *what)
      : std::runtime_error(what)
    { }
};


/* InstructionDecoder component to be used by class Processor */

/*
  * A thing to understand is that the location of these fields is in general always the same, except for some cases. 
  * What does change however is what you will be getting.
  * So which combination of getters you will be using. And this depends on on the instruction type that we are using.
  * 
  * */
class InstructionDecoder
{
  public:
    /**
     * Set the instruction word for decoding.
     * @param instructionWord The 32-bit instruction word.
     */
    void setInstructionWord(const uint32_t instructionWord);

    /**
     * Get the instruction word currently set for decoding.
     * @return The 32-bit instruction word.
     */
    uint32_t getInstructionWord() const;

    // /**
    //  * Set the instruction type.
    //  * @param instructionType The type of the instruction (R, I, S, SH, J, F).
    //  */
    // void setInstructionType(const Type instructionType);

    /**
     * Get the instruction type.
     * @return The type of the instruction.
     */
    InstructionType getInstructionType() const;

    /**
     * Get the opcode of the instruction.
     * @brief gets the bits at the bit positions 31-26
     * in the case of a R, I, S, SH or F type instruction
     * @return The opcode.
     */
    uint16_t getOpcode() const;
    
    /**
     * Get the op2 field of the instruction.
     * @brief gets the bits at the bit positions 9-8
     * in the case of an R type instruction or the bits
     * 7-6 in the case of an SH type instruction
     * @return The op2 field.
     */
    uint8_t getOp2() const;

    /**
     * Get the op3 field of the instruction.
     * @brief gets the bits at the bit positions 3-0
     * in the case of a R type instruction
     * @return The op3 field.
     */
    uint8_t getOp3() const;

    /**
     * Get FunctionCode of the instruction.
     * @brief gets the bits at the bit positions
     * in the case of a R type instruction
     * @return The FunctionCode.
     */
    uint8_t getFunctionCode() const;
    
    /**
     * Get the value of register A.
     * @brief gets the bits at the bit positions 20-16
     * in the case of a R, I, S, SH or F type instruction
     * @return The value of register A.
     */
    RegNumber getA() const;

    /**
     * Get the value of register B.
     * @brief gets the bits at the bit positions 15-11
     * in the case of a R or S type instruction
     * @return The value of register B.
     */
    RegNumber getB() const;

    /**
     * Get the value of register D.
     * @brief gets the bits at the bit positions 25-21
     * in the case of a R, I or SH type instruction
     * @return The value of register D.
     */
    RegNumber getD() const;

    /**
     * Get the immediate value.
     * @brief gets the bits at the bit positions 15-0
     * in the case of a I or F type instruction or the bits
     * 10-0 in the case of an S type instruction
     * @return The immediate value I.
     */
    uint16_t getImmediateI() const;

    /**
     * Get the immediate value.
     * @brief gets the bits at the bit positions 25-0
     * in the case of a J type instruction 
     * @return The immediate N.
     */
    uint32_t getImmediateN() const;

    /**
     * Get the reserved field of the instruction.
     * @brief gets the bits at the bit position 10 and 7-4
     * in the case of a R type instruction or the bits
     * 15-8 in the case of an S type instruction
     * @return The reserved field.
     */
    uint8_t getReserved() const;

    /**
     * Get the L field of the instruction.
     * @brief gets the bits at the bit positions 5-0
     * in the case of a SH type instruction
     * @return The L field.
     */
    uint8_t getL() const;

    /**
     * @brief 
     * 
     * @return uint32_t 
     */ */
    uint32_t getK() const;

    // /**
    //  * @brief 
    //  * 
    //  * @return uint8_t 
    //  */ */
    // uint8_t getO() const;

  private:
    uint32_t instructionWord;
    InstructionType instructionType;
};

std::ostream &operator<<(std::ostream &os, const InstructionDecoder &decoder);

#endif /* __INST_DECODER_H__ */
