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

void
InstructionDecoder::setInstructionType(const Type instructionType) 
{
  this->instructionType = instructionType;
}

Type
InstructionDecoder::getInstructionType() const
{
  return instructionType;
}

RegNumber
InstructionDecoder::getA() const
{
  uint32_t value = getInstructionWord();
  uint32_t mask = BITS_20_16;
  RegNumber A = (value & mask) >> 16; // Right shift by 8 bits
  return A;  /* result undefined */
}

RegNumber
InstructionDecoder::getB() const
{
  /* TODO: implement */

  return 0;  /* result undefined */
}

RegNumber
InstructionDecoder::getD() const
{
  /* TODO: implement */

  return 0; /* result undefined */
}

uint16_t
InstructionDecoder::getOpcode() const
{
  /* TODO: implement */

  return 0; /* result undefined */
}

uint8_t
InstructionDecoder::getOp2() const
{
  /* TODO: implement */

  return 0; /* result undefined */
}

uint8_t
InstructionDecoder::getOp3() const
{
  /* TODO: implement */

  return 0; /* result undefined */
}

uint16_t
InstructionDecoder::getImmediateI() const
{
  /* TODO: implement */

  return 0; /* result undefined */
}

uint32_t
InstructionDecoder::getImmediateN() const
{
  /* TODO: implement */

  return 0; /* result undefined */
}

uint8_t
InstructionDecoder::getReserved() const
{
  /* TODO: implement */

  return 0; /* result undefined */
}

uint8_t
InstructionDecoder::getL() const
{
  /* TODO: implement */

  return 0; /* result undefined */
}
