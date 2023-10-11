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
  InstructionType instructionType = INVALID;
  // depending on the opcode we return a certain instruction type
  uint8_t opCode = (getInstructionWord() >> 26);

  switch (opCode) {
    case 0x00:
    case 0x01:
    case 0x03:
    case 0x04:
      instructionType = J;
      break;
    case 0x02:
      instructionType = DN;
      break;
    case 0x05:
      instructionType = ORK;
      break;
    case 0x06:
      instructionType = DROK;
      break;
    case 0x08:
      instructionType = OK;
      break;
    case 0x09:
    case 0x1C:
    case 0x1D:
    case 0x1E:
    case 0x1F:
    case 0x3D:
    case 0x3E:
    case 0x3F:
      instructionType = RES;
      break;
    case 0x0A:
    case 0x32:
      instructionType = DABROO;
      break;
    case 0x11:
    case 0x12:
      instructionType = RBR;
      break;
    case 0x13:
      instructionType = RAI;
      break;
    case 0x1A:
    case 0x1B:
    case 0x20:
    case 0x21:
    case 0x22:
    case 0x23:
    case 0x24:
    case 0x25:
    case 0x26:
    case 0x27:
    case 0x28:
    case 0x2B:
    case 0x2C:
      instructionType = I;
      break;
    case 0x29:
    case 0x2A:
    case 0x2D:
      instructionType = DAK;
      break;
    case 0x2E:
      instructionType = SH;
      break;
    case 0x2F:
      instructionType = F;
      break;
    case 0x30:
      instructionType = KABK;
      break;
    case 0x31:
      instructionType = RABRO;
      break;
    case 0x33:
    case 0x35:
    case 0x36:
    case 0x37:
      instructionType = S;
      break;
    case 0x38:
      instructionType = R;
      break;
    case 0x39:
      instructionType = OABR;
      break;
    case 0x3C:
      instructionType = DABLK;
      break;
  }

  return instructionType;
}

uint16_t
InstructionDecoder::getOpcode() const
{
  uint16_t opcode = 0;

  switch (getInstructionType()) {
    case F:
      opcode = (getInstructionWord() & BITS_31_21) >> 21; // Right shift by 8 bits
      break;
    default:
      opcode = (getInstructionWord() & BITS_31_26) >> 26; // Right shift by 8 bits
      break;
  }

  return opcode;  /* result undefined */
}

uint32_t
InstructionDecoder::getOp2() const
{
  uint32_t op2 = FIELD_NOT_AVAILABLE_32_BIT;

  switch (getInstructionType()) {
    case R:
      op2 = (getInstructionWord() & BITS_9_8) >> 8; // Right shift by 8 bits
      break;
    case SH:
      op2 = (getInstructionWord() & BITS_7_6) >> 6; // Right shift by 8 bits
      break;
    case ORK:
      op2 = (getInstructionWord() & BITS_25_24) >> 24; // Right shift by 8 bits
      break;
    case DROK:
      op2 = (getInstructionWord() & BITS_16) >> 16; // Right shift by 8 bits
      break;
    case OK:
      op2 = (getInstructionWord() & BITS_25_16) >> 16; // Right shift by 8 bits
      if ((op2 >> 9) == 1) op2 = (getInstructionWord() & BITS_25_0) >> 0; // Right shift by 8 bits
      break;
    case DABROO:
      op2 = (getInstructionWord() & BITS_7_4) >> 4; // Right shift by 8 bits
      if (op2 < 0b1100) op2 = (getInstructionWord() & BITS_7_0) >> 0;
      break;
    case RABRO:
      op2 = (getInstructionWord() & BITS_3_0) >> 0;
      break;
    case OABR:
      op2 = (getInstructionWord() & BITS_25_21) >> 21; // Right shift by 8 bits
      break;
    default:
      op2 = FIELD_NOT_AVAILABLE_32_BIT;
  }

  return op2;  /* result undefined */
}

uint8_t
InstructionDecoder::getOp3() const
{
  uint8_t op3 = FIELD_NOT_AVAILABLE_8_BIT;

  switch (getInstructionType()) {
    case R:
      op3 = (getInstructionWord() & BITS_3_0) >> 0; // Right shift by 8 bits
      break;
    default:
      op3 = FIELD_NOT_AVAILABLE_8_BIT;
  }

  return op3;  /* result undefined */
}


// function that maps opcode + op2 + op3 to an enum
// uint8_t
// InstructionDecoder::getFunctionCode() const
// {
//   uint8_t op2 = 0;

//   switch (getInstructionType()) {
//     case R:
//       op2 = (getOp2() << 4) | getOp3(); // Right shift by 8 bits
//       break;
//     case SH:
//       op2 = getOp2(); // Right shift by 8 bits
//       break;
//     case ORK:
//       break;
//     case DROK:
//       break;
//     case OK:
//       break;
//     case DABROO:
//       break;
//     case OABR:
//       break;
//     default:
//       op2 = FIELD_NOT_AVAILABLE_8_BIT;
//   }

//   return op2;  /* result undefined */
// }

RegNumber
InstructionDecoder::getA() const
{
  RegNumber A = FIELD_NOT_AVAILABLE_8_BIT;

  switch (getInstructionType()) {
    case R:
    case I:
    case S:
    case SH:
    case F:
    case DABROO:
    case RAI:
    case DAK:
    case KABK:
    case RABRO:
    case OABR:
    case DABLK:
      A = (getInstructionWord() & BITS_20_16) >> 16; // Right shift by 8 bits
      break;
    default:
      A = FIELD_NOT_AVAILABLE_8_BIT;
  }

  return A;  /* result undefined */
}

RegNumber
InstructionDecoder::getB() const
{
  RegNumber B = FIELD_NOT_AVAILABLE_8_BIT;

  switch (getInstructionType()) {
    case R:
    case S:
    case DABROO:
    case RBR:
    case KABK:
    case RABRO:
    case OABR:
    case DABLK:
      B = (getInstructionWord() & BITS_15_11) >> 11; // Right shift by 8 bits
      break;
    default:
      B = FIELD_NOT_AVAILABLE_8_BIT;
  }

  return B;  /* result undefined */
}

RegNumber
InstructionDecoder::getD() const
{
  RegNumber D = FIELD_NOT_AVAILABLE_8_BIT;

  switch (getInstructionType()) {
    case R:
    case I:
    case SH:
    case DN:
    case DROK:
    case DABROO:
    case DAK:
    case DABLK:
      D = (getInstructionWord() & BITS_25_21) >> 21; // Right shift by 8 bits
      break;
    default:
      D = FIELD_NOT_AVAILABLE_8_BIT;
  }

  return D;  /* result undefined */
}

uint16_t
InstructionDecoder::getImmediateI() const
{
  uint16_t immediateI = FIELD_NOT_AVAILABLE_16_BIT;

  switch (getInstructionType()) {
    case I:
    case F:
    case RAI:
      immediateI = (getInstructionWord() & BITS_15_0) >> 0; // Right shift by 8 bits
      break;
    case S:
      immediateI = (getInstructionWord() & BITS_10_0) >> 0; // Right shift by 8 bits
      break;
    default:
      immediateI = FIELD_NOT_AVAILABLE_16_BIT;
  }

  return immediateI;  /* result undefined */
}

uint32_t
InstructionDecoder::getImmediateN() const
{
  uint32_t immediateN = FIELD_NOT_AVAILABLE_32_BIT;

  switch (getInstructionType()) {
    case J:
      immediateN = (getInstructionWord() & BITS_25_0) >> 0; // Right shift by 8 bits
      break;
    case DN:
      immediateN = (getInstructionWord() & BITS_20_0) >> 0; // Right shift by 8 bits
      break;
    default:
      immediateN = FIELD_NOT_AVAILABLE_32_BIT;
  }

  return immediateN;  /* result undefined */
}

uint32_t
InstructionDecoder::getReserved() const
{
  uint32_t reserved = FIELD_NOT_AVAILABLE_32_BIT;

  switch (getInstructionType()) {
    case R:
      reserved = (((getInstructionWord() & BITS_10) >> 10) << 4) | ((getInstructionWord() & BITS_7_4) >> 4); // Right shift by 8 bits
      break;
    case SH:
      reserved = (getInstructionWord() & BITS_15_8) >> 8; // Right shift by 8 bits
      break;
    case ORK:
      reserved = (getInstructionWord() & BITS_23_16) >> 16; // Right shift by 8 bits
      break;
    case DROK:
      reserved = (getInstructionWord() & BITS_20_17) >> 17; // Right shift by 8 bits
      break;
    case RES:
      reserved = (getInstructionWord() & BITS_25_0) >> 0; // Right shift by 8 bits
      break; 
    case DABROO:
      if (getOpcode() == 0x0A) {
        if (getOp2() >= 0b1100 && getOp2() <= 0b1111)          
          reserved = (((getInstructionWord() & BITS_25_8) >> 8) << 4) | ((getInstructionWord() & BITS_3_0)); // Right shift by 8 bits
        else
          reserved = (getInstructionWord() & BITS_10_8) >> 8; // Right shift by 8 bits
      } else if (getOpcode() == 0x32) {
        if((getOp2() >= 0b1000 && getOp2() <= 0b1101 && (((getInstructionWord() >> 7) & 0b1) == 0)) || (getOp2() >= 0b00101000 && getOp2() <= 0b00101110))
          reserved = (((getInstructionWord() & BITS_25_21) >> 21) << 3) | ((getInstructionWord() & BITS_10_8) >> 8);
        else if((getOp2() >= 0b00011000 && getOp2() <= 0b00011101) || (getOp2() >= 0b111000 && getOp2() <= 0b00111110))
          reserved = (((getInstructionWord() & BITS_25_21) >> 21) << 1) | ((getInstructionWord() & BITS_10) >> 10);
        else if(getOp2() == 0b00110100)
          reserved = (((getInstructionWord() & BITS_25_21) >> 21) << 2) | ((getInstructionWord() & BITS_9_8) >> 8);
        else if(getOp2() == 0b00110101)
          reserved = (((getInstructionWord() & BITS_25_21) >> 21) << 2) | ((getInstructionWord() & BITS_10) >> 10)  << 1 | ((getInstructionWord() & BITS_8) >> 8);
        else if(getOp2() == 0b1101 || getOp2() == 0b1110)
          reserved = (((getInstructionWord() & BITS_25_21) >> 21) << 4) | ((getInstructionWord() & BITS_3_0) >> 0);
        
        else if(getOp2() >= 0b00000000 && getOp2() <= 0b00000111)
          reserved = (getInstructionWord() & BITS_10_8) >> 8; // Right shift by 8 bits
        else if(getOp2() == 0b00010101 || getOp2() == 0b00010100)
          reserved = (getInstructionWord() & BITS_8) >> 8; // Right shift by 8 bits
      }
      break;
    case RBR:
      reserved = (((getInstructionWord() & BITS_25_16) >> 16) << 11) | ((getInstructionWord() & BITS_10_0) >> 0);
      break;
    case RAI:
      reserved = (getInstructionWord() & BITS_25_21) >> 21; // Right shift by 8 bits
      break;
    case RABRO:
      reserved = (((getInstructionWord() & BITS_25_21) >> 21) << 7) | ((getInstructionWord() & BITS_10_4) >> 4);
      break;
    case OABR:
      reserved = (getInstructionWord() & BITS_10_0) >> 0; // Right shift by 8 bits
      break;
    default:
      reserved = FIELD_NOT_AVAILABLE_32_BIT;
  }

  return reserved;  /* result undefined */
}

uint8_t
InstructionDecoder::getL() const
{
  uint8_t L = FIELD_NOT_AVAILABLE_8_BIT;

  switch (getInstructionType()) {
    case SH:
      L = (getInstructionWord() & BITS_5_0) >> 0; // Right shift by 8 bits
      break;
    case DABLK:
      L = (getInstructionWord() & BITS_10_5) >> 5; // Right shift by 8 bits
      break;
    default:
      L = FIELD_NOT_AVAILABLE_8_BIT;
  }

  return L;  /* result undefined */
}


uint32_t
InstructionDecoder::getK() const
{
  uint32_t K = FIELD_NOT_AVAILABLE_32_BIT;

  switch (getInstructionType()) {
    case DABLK:
      K = (getInstructionWord() & BITS_5_0) >> 0; // Right shift by 8 bits
      break;
    case ORK:
    case DROK:
    case OK:
    case DAK:
      K = (getInstructionWord() & BITS_15_0) >> 0; // Right shift by 8 bits
      break;
    case KABK:
      K = (((getInstructionWord() & BITS_25_21) >> 21) << 11) | ((getInstructionWord() & BITS_10_0));
      break;
    default:
      K = FIELD_NOT_AVAILABLE_32_BIT;
  }

  return K;  /* result undefined */
}


uint8_t
InstructionDecoder::getO() const
{
  uint8_t O = FIELD_NOT_AVAILABLE_8_BIT;

  switch (getOpcode()) {
    case 0x32:
      if(getOp2() == 0b00110100)
        O = (getInstructionWord() & BITS_10) >> 10; // Right shift by 8 bits
      else if(getOp2() == 0b00110101)
        O = (getInstructionWord() & BITS_9) >> 9; // Right shift by 8 bits
      if((getOp2() >= 0b00011000 && getOp2() <= 0b00011101) || (getOp2() >= 0b111000 && getOp2() <= 0b111110))
        O = (getInstructionWord() & BITS_9_8) >> 8; // Right shift by 8 bits
      else if(getOp2() == 0b00010100 || getOp2() == 0b00010101)
        O = (getInstructionWord() & BITS_10_9) >> 9; // Right shift by 8 bits
      else if(getOp2() == 0b1101 || getOp2() == 0b1110)
        O = (getInstructionWord() & BITS_10_8) >> 8; // Right shift by 8 bits
      else if(getOp2() >= 0b00010000 && getOp2() <= 0b00010111)
        O = (getInstructionWord() & BITS_10_8) >> 8; // Right shift by 8 bits
      break;
    default:
      O = FIELD_NOT_AVAILABLE_8_BIT;
  }

  return O;  /* result undefined */
}