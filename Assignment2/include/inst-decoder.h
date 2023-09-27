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
#define BITS_25_21 0x03E00000   // Represents 00000000 00111110 00000000 00000000
#define BITS_25_0  0x03FFFFFF   // Represents 00000000 00111111 11111111 11111111
#define BITS_20_16 0x001F0000   // Represents 00000000 00000000 00011111 00000000
#define BITS_15_11 0x0000F800   // Represents 00000000 00000000 00001111 10000000
#define BITS_15_8  0x0000FF00   // Represents 00000000 00000000 11111111 00000000
#define BITS_15_0  0x0000FFFF   // Represents 00000000 00000000 11111111 11111111
#define BITS_10_0  0x000007FF   // Represents 00000000 00000000 00000111 11111111
#define BITS_10    0x00000400   // Represents 00000000 00000000 00000100 00000000
#define BITS_9_8   0x00000300   // Represents 00000000 00000000 00000011 00000000
#define BITS_7_6   0x000000C0   // Represents 00000000 00000000 00000000 11000000
#define BITS_7_4   0x000000F0   // Represents 00000000 00000000 00000000 11110000
#define BITS_5_0   0x0000003F   // Represents 00000000 00000000 00000000 00111111
#define BITS_3_0   0x0000000F   // Represents 00000000 00000000 00000000 00001111

enum Type {
  R,
  I,
  S,
  SH,
  J,
  F
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
class InstructionDecoder
{
  public:
    void                setInstructionWord(const uint32_t instructionWord);
    uint32_t            getInstructionWord() const;
    
    void                setInstructionType(const Type instructionType);
    Type                getInstructionType() const;

    /*
     * A thing to understand is that the location of these fields is in gereral always the same, except for some cases. 
     * What does change however is what you will be getting.
     * So which combination of getters you will be using. And this depends on on the instruction type that we are using.*/

    RegNumber           getA() const;
    RegNumber           getB() const;
    RegNumber           getD() const;

    /* TODO: probably want methods to get opcode, function code */
    uint16_t            getOpcode() const;
    uint8_t             getOp2() const;
    uint8_t             getOp3() const;

    /* TODO: need a method to obtain the immediate */
    uint16_t            getImmediateI() const;
    uint32_t            getImmediateN() const;

    /* Methods for getting reserved and L fields */
    uint8_t             getReserved() const;
    uint8_t             getL() const;

  private:
    uint32_t instructionWord;
    Type instructionType;
};

std::ostream &operator<<(std::ostream &os, const InstructionDecoder &decoder);

#endif /* __INST_DECODER_H__ */
