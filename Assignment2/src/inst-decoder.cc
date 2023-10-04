/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    inst-decoder.cc - RISC-V instruction decoder.
 *
 * Copyright (C) 2016,2019  Leiden University, The Netherlands.
 *
 */

#include "inst-decoder.h"

#include <map>

/*
 * Class InstructionDecoder -- helper class for getting specific
 * information from the decoded instruction.
 */

void
InstructionDecoder::setInstructionWord(const uint32_t instructionWord)
{
  this->instructionWord = instructionWord;
}

uint32_t
InstructionDecoder::getInstructionWord() const
{
  return instructionWord;
}

// void
// InstructionDecoder::setInstructionType(const Type instructionType) 
// {
//   this->instructionType = instructionType;
// }

InstructionType
InstructionDecoder::getInstructionType() const
{
  InstructionType instructionType = R;
  // depending on the opcode we return a certain instruction type
  uint32_t instructionWord = getInstructionWord();
  uint8_t opCode = (instructionWord >> 26);

  if (opCode == 0x38) {
    instructionType = (InstructionType) R;
  } else if (opCode >= 0x1A && opCode <= 0x2C) {
    instructionType = (InstructionType) I;
  } else if (opCode >= 0x33 && opCode <= 0x37) {
    instructionType = (InstructionType) S;
  } else if (opCode == 0x2E) {
    instructionType = (InstructionType) SH;
  } else if (opCode >= 0x00 && opCode <= 0x04) {
    instructionType = (InstructionType) J;
  } else if (opCode == 0x2F) {
    instructionType = (InstructionType) F;
  }
  return instructionType;
}

RegNumber
InstructionDecoder::getA() const
{
  RegNumber A = 0;

  switch (getInstructionType()) {
    case R:
    case I:
    case S:
    case SH:
    case F:
      A = (getInstructionWord() & BITS_20_16) >> 16; // Right shift by 8 bits
      break;
    case J:
    default:
      A = FIELD_NOT_AVAILABLE_8_BIT;
  }

  return A;  /* result undefined */
}

RegNumber
InstructionDecoder::getB() const
{
  RegNumber B = 0;

  switch (getInstructionType()) {
    case R:
    case S:
      B = (getInstructionWord() & BITS_15_11) >> 11; // Right shift by 8 bits
      break;
    case I:
    case SH:
    case F:
    case J:
    default:
      B = FIELD_NOT_AVAILABLE_8_BIT;
  }

  return B;  /* result undefined */
}

RegNumber
InstructionDecoder::getD() const
{
  RegNumber D = 0;

  switch (getInstructionType()) {
    case R:
    case I:
    case SH:
      D = (getInstructionWord() & BITS_25_21) >> 21; // Right shift by 8 bits
      break;
    case S:
    case F:
    case J:
    default:
      D = FIELD_NOT_AVAILABLE_8_BIT;
  }

  return D;  /* result undefined */
}

uint16_t
InstructionDecoder::getOpcode() const
{
  uint16_t opcode = 0;

  switch (getInstructionType()) {
    case R:
    case I:
    case SH:
    case S:
    case J:
      opcode = (getInstructionWord() & BITS_31_26) >> 26; // Right shift by 8 bits
      break;
    case F:
      opcode = (getInstructionWord() & BITS_31_21) >> 21; // Right shift by 8 bits
      break;
    default:
      opcode = FIELD_NOT_AVAILABLE_16_BIT;
  }

  return opcode;  /* result undefined */
}

uint8_t
InstructionDecoder::getOp2() const
{
  uint8_t op2 = 0;

  switch (getInstructionType()) {
    case R:
      op2 = (getInstructionWord() & BITS_9_8) >> 8; // Right shift by 8 bits
      break;
    case SH:
      op2 = (getInstructionWord() & BITS_7_6) >> 6; // Right shift by 8 bits
      break;
    case I:
    case S:
    case F:
    case J:
    default:
      op2 = FIELD_NOT_AVAILABLE_8_BIT;
  }

  return op2;  /* result undefined */
}

uint8_t
InstructionDecoder::getOp3() const
{
  uint8_t op3 = 0;

  switch (getInstructionType()) {
    case R:
      op3 = (getInstructionWord() & BITS_3_0) >> 0; // Right shift by 8 bits
      break;
    case SH:
    case I:
    case S:
    case F:
    case J:
    default:
      op3 = FIELD_NOT_AVAILABLE_8_BIT;
  }

  return op3;  /* result undefined */
}


uint8_t
InstructionDecoder::getFunctionCode() const
{
  uint8_t op2 = 0;

  switch (getInstructionType()) {
    case R:
      op2 = (getOp2() << 4) | getOp3(); // Right shift by 8 bits
      break;
    case SH:
      op2 = getOp2(); // Right shift by 8 bits
      break;
    case I:
    case S:
    case F:
    case J:
    default:
      op2 = FIELD_NOT_AVAILABLE_8_BIT;
  }

  return op2;  /* result undefined */
}

uint16_t
InstructionDecoder::getImmediateI() const
{
  uint16_t immediateI = 0;

  switch (getInstructionType()) {
    case I:
    case F:
      immediateI = (getInstructionWord() & BITS_15_0) >> 0; // Right shift by 8 bits
      break;
    case S:
      immediateI = (getInstructionWord() & BITS_10_0) >> 0; // Right shift by 8 bits
      break;
    case R:
    case SH:
    case J:
    default:
      immediateI = FIELD_NOT_AVAILABLE_16_BIT;
  }

  return immediateI;  /* result undefined */
}

uint32_t
InstructionDecoder::getImmediateN() const
{
  uint32_t immediateN = 0;

  switch (getInstructionType()) {
    case J:
      immediateN = (getInstructionWord() & BITS_25_0) >> 0; // Right shift by 8 bits
      break;
    case R:
    case S:
    case SH:
    case I:
    case F:
    default:
      immediateN = FIELD_NOT_AVAILABLE_32_BIT;
  }

  return immediateN;  /* result undefined */
}

uint8_t
InstructionDecoder::getReserved() const
{
  uint8_t reserved = 0;

  switch (getInstructionType()) {
    case R:
      reserved = (((getInstructionWord() & BITS_10) >> 10) << 4) | ((getInstructionWord() & BITS_7_4) >> 4); // Right shift by 8 bits
      break;
    case SH:
      reserved = (getInstructionWord() & BITS_15_8) >> 8; // Right shift by 8 bits
      break;
    case S:
    case I:
    case F:
    case J:
    default:
      reserved = FIELD_NOT_AVAILABLE_8_BIT;
  }

  return reserved;  /* result undefined */
}

uint8_t
InstructionDecoder::getL() const
{
  uint8_t L = 0;

  switch (getInstructionType()) {
    case SH:
      L = (getInstructionWord() & BITS_5_0) >> 0; // Right shift by 8 bits
      break;
    case I:
    case R:
    case S:
    case F:
    case J:
    default:
      L = FIELD_NOT_AVAILABLE_8_BIT;
  }

  return L;  /* result undefined */
}
