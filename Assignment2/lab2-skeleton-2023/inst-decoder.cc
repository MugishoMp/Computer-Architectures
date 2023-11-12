/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    inst-decoder.cc - RISC-V instruction decoder.
 *
 * Copyright (C) 2016,2019  Leiden University, The Netherlands.
 *
 */

#include "inst-decoder.h"

#include <map>

#include<iostream>

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
    default:
      throw IllegalInstruction("This opcode does not exist");
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
      if ((getInstructionWord() & 0b1000) == 0b1000) op2 = (getInstructionWord() & BITS_9_6) >> 6; // Right shift by 8 bits
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
      throw IllegalInstruction("Op2 field not available for this instruction type");
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
      throw IllegalInstruction("Op3 field not available for this instruction type");
  }

  return op3;  /* result undefined */
}

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
      throw IllegalInstruction("A register field not available for this instruction type");
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
      throw IllegalInstruction("B register field not available for this instruction type");
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
      throw IllegalInstruction("D register field not available for this instruction type");
  }
  return D;  /* result undefined */
}

// can be a negative number
int16_t
InstructionDecoder::getImmediateI() const
{
  int16_t immediateI = FIELD_NOT_AVAILABLE_16_BIT;

  switch (getInstructionType()) {
    case I:
    case F:
    case RAI:
      immediateI = (getInstructionWord() & BITS_15_0) >> 0; // Right shift by 8 bits
      break;
    case S:
      // we need to sign extend this field because here we are trying to fit 
      // a smaller SIGNED number in a bigger "container"!
      immediateI = (getInstructionWord() & BITS_10_0) >> 0; // Right shift by 8 bits
      if (immediateI >> 10 & 0b1) immediateI = immediateI | (0b11111 << 11);
      break;
    default:
      throw IllegalInstruction("Immediate I field not available for this instruction type");
  }

  return immediateI;  /* result undefined */
}

// can be a negative number
int32_t
InstructionDecoder::getImmediateN() const
{
  int32_t immediateN = FIELD_NOT_AVAILABLE_32_BIT;

  switch (getInstructionType()) {
    case J:
      immediateN = (getInstructionWord() & BITS_25_0) >> 0; // Right shift by 8 bits
      if (immediateN >> 25 & 0b1) immediateN = immediateN | (0b111111 << 26);
      break;
    case DN:
      immediateN = (getInstructionWord() & BITS_20_0) >> 0; // Right shift by 8 bits
      if (immediateN >> 20 & 0b1) immediateN = immediateN | (0b11111111111 << 21);
      break;
    default:
      throw IllegalInstruction("Immediate N field not available for this instruction type");
  }

  return immediateN;  /* result undefined */
}

// can be a negative number
int32_t
InstructionDecoder::getImmediate() const
{
  int32_t immediate = FIELD_NOT_AVAILABLE_32_BIT;

  switch (getInstructionType()) {
    case I:
    case F:
    case S:
      immediate = static_cast<int32_t>(getImmediateI());
      break;
    case J:
    case DN:
      immediate = static_cast<int32_t>(getImmediateN());
      break;
    // case DABLK:
    case ORK:
    case DROK:
    case OK:
    case DAK:
    case KABK:
      immediate = static_cast<int32_t>(getK());
      break;
    case SH:
    // case DABLK:
      immediate = static_cast<int32_t>(getL());
      break;
    default:
      throw IllegalInstruction("Immediate N field not available for this instruction type");
  }

  return immediate;  /* result undefined */
}

int32_t
InstructionDecoder::getReserved() const
{
  int32_t reserved = FIELD_NOT_AVAILABLE_32_BIT;

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
      throw IllegalInstruction("Reserved field not available for this instruction type");
  }

  return reserved;  /* result undefined */
}

int8_t
InstructionDecoder::getL() const
{
  int8_t L = FIELD_NOT_AVAILABLE_8_BIT;

  switch (getInstructionType()) {
    case SH:
      L = (getInstructionWord() & BITS_5_0) >> 0; // Right shift by 8 bits
      break;
    case DABLK:
      L = (getInstructionWord() & BITS_10_5) >> 5; // Right shift by 8 bits
      break;
    default:
      throw IllegalInstruction("L field not available for this instruction type");
  }

  return L;  /* result undefined */
}


int32_t
InstructionDecoder::getK() const
{
  int32_t K = FIELD_NOT_AVAILABLE_32_BIT;

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
      throw IllegalInstruction("K field not available for this instruction type");
  }

  return K;  /* result undefined */
}


int8_t
InstructionDecoder::getO() const
{
  int8_t O = FIELD_NOT_AVAILABLE_8_BIT;

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
      throw IllegalInstruction("O field not available for this instruction type");
  }

  return O;  /* result undefined */
}

InstructionMnemonic 
InstructionDecoder::getFunctionCode() const
{
  InstructionMnemonic functionCode = InstructionMnemonic::INVALID;
  switch (getInstructionType()) {
    case R:
      functionCode = getFunctionCodeRTypeInstruction();
      break;
    case I:
      functionCode = getFunctionCodeITypeInstruction();
      break;
    case S:
      functionCode = getFunctionCodeSTypeInstruction();
      break;
    case SH:
      functionCode = getFunctionCodeSHTypeInstruction();
      break;
    case J:
      functionCode = getFunctionCodeJTypeInstruction();
      break;
    case F:
      functionCode = getFunctionCodeFTypeInstruction();
      break;
    case DN:
      functionCode = getFunctionCodeDNTypeInstruction();
      break;
    case ORK:
      functionCode = getFunctionCodeORKTypeInstruction();
      break;
    case DROK:
      functionCode = getFunctionCodeDROKTypeInstruction();
      break;
    case OK:
      functionCode = getFunctionCodeOKTypeInstruction();
      break;
    case RES:
      functionCode = getFunctionCodeRESTypeInstruction();
      break;
    // will take too much time i think  
    // case DABROO:
    //   functionCode = getFunctionCodeDABROOTypeInstruction();
    //   break;
    case RBR:
      functionCode = getFunctionCodeRBRTypeInstruction();
      break;
    case RAI:
      functionCode = getFunctionCodeRAITypeInstruction();
      break;
    case DAK:
      functionCode = getFunctionCodeDAKTypeInstruction();
      break;
    case KABK:
      functionCode = getFunctionCodeKABKTypeInstruction();
      break;
    case RABRO:
      functionCode = getFunctionCodeRABROTypeInstruction();
      break;
    case OABR:
      functionCode = getFunctionCodeOABRTypeInstruction();
      break;
    case DABLK:
      functionCode = getFunctionCodeDABLKTypeInstruction();
      break;
    case INVALID:
    default:
      throw IllegalInstruction("This instruction is invalid");
  }
  return functionCode;
}

InstructionMnemonic 
InstructionDecoder::getFunctionCodeRTypeInstruction() const
{// 0x38
  InstructionMnemonic functionCode = InstructionMnemonic::INVALID;
  switch(getOp3()) {
    case 0b0000:
      functionCode = InstructionMnemonic::L_ADD;
      break;
    case 0b0001:
      functionCode = InstructionMnemonic::L_ADDC;
      break;
    case 0b0010:
      functionCode = InstructionMnemonic::L_SUB;
      break;
    case 0b0011:
      functionCode = InstructionMnemonic::L_AND;
      break;
    case 0b0100:
      functionCode = InstructionMnemonic::L_OR;
      break;
    case 0b0101:
      functionCode = InstructionMnemonic::L_XOR;
      break;
    case 0b0110:
      functionCode = InstructionMnemonic::L_MUL;
      break;
    case 0b0111:
      functionCode = InstructionMnemonic::L_MULD;
      break;
    case 0b1000:
        // std::cout << "CHECKSOUT" << std::endl;
        // std::cout << std::hex << getOp2() << std::endl;
      switch (getOp2()) {
        case 0b0000:
          functionCode = InstructionMnemonic::L_SLL;
          break;
        case 0b0001:
          functionCode = InstructionMnemonic::L_SRL;
          break;
        case 0b0010:
          functionCode = InstructionMnemonic::L_SRA;
          break;
        case 0b0011:
          functionCode = InstructionMnemonic::L_ROR;
          break;
      }
      break;
    case 0b1001:
      functionCode = InstructionMnemonic::L_DIV;
      break;
    case 0b1010:
      functionCode = InstructionMnemonic::L_DIVU;
      break;
    case 0b1011:
      functionCode = InstructionMnemonic::L_MULU;
      break;
    case 0b1100:
      switch (getOp2()) {
        case 0b0000:
          functionCode = InstructionMnemonic::L_EXTHS;
          break;
        case 0b0001:
          functionCode = InstructionMnemonic::L_EXTBS;
          break;
        case 0b0010:
          functionCode = InstructionMnemonic::L_EXTHZ;
          break;
        case 0b0011:
          if (!((getInstructionWord() << 9) | 0b1))
            functionCode = InstructionMnemonic::L_EXTBZ;
          else
            functionCode = InstructionMnemonic::L_MULDU;
          break;
      }
      break;
    case 0b1101:
      if (getOp2() == 0b0000)
        functionCode = InstructionMnemonic::L_EXTWS;
      else
        functionCode = InstructionMnemonic::L_EXTWZ;
      break;
    case 0b1110:
      functionCode = InstructionMnemonic::L_CMOV;
      break;
    case 0b1111:
      switch (getOp2()) {
        case 0b00:
          functionCode = InstructionMnemonic::L_FF1;
          break;
        case 0b01:
          functionCode = InstructionMnemonic::L_FL1;
          break;
      }
  }
  return functionCode;
}

InstructionMnemonic 
InstructionDecoder::getFunctionCodeITypeInstruction() const
{
  InstructionMnemonic functionCode = InstructionMnemonic::INVALID;
  switch(getOpcode()) {
    case 0x1A:
      functionCode = InstructionMnemonic::L_LF;
      break;
    case 0x1B:
      functionCode = InstructionMnemonic::L_LWA;
      break;
    case 0x20:
      functionCode = InstructionMnemonic::L_LD;
      break;
    case 0x21:
      functionCode = InstructionMnemonic::L_LWZ;
      break;
    case 0x22:
      functionCode = InstructionMnemonic::L_LWS;
      break;
    case 0x23:
      functionCode = InstructionMnemonic::L_LBZ;
      break;
    case 0x24:
      functionCode = InstructionMnemonic::L_LBS;
      break;
    case 0x25:
      functionCode = InstructionMnemonic::L_LHZ;
      break;
    case 0x26:
      functionCode = InstructionMnemonic::L_LHS;
      break;
    case 0x27:
      functionCode = InstructionMnemonic::L_ADDI;
      break;
    case 0x28:
      functionCode = InstructionMnemonic::L_ADDIC;
      break;
    case 0x2B:
      functionCode = InstructionMnemonic::L_XORI;
      break;
    case 0x2C:
      functionCode = InstructionMnemonic::L_MULI;
      break;
  }
  return functionCode;
}

InstructionMnemonic 
InstructionDecoder::getFunctionCodeSTypeInstruction() const
{
  InstructionMnemonic functionCode = InstructionMnemonic::INVALID;
  switch(getOpcode()) {
    case 0x33:
      functionCode = InstructionMnemonic::L_SWA;
      break;
    case 0x35:
      functionCode = InstructionMnemonic::L_SW;
      break;
    case 0x36:
      functionCode = InstructionMnemonic::L_SB;
      break;
    case 0x37:
      functionCode = InstructionMnemonic::L_SH;
      break;
  }
  return functionCode;
}

InstructionMnemonic 
InstructionDecoder::getFunctionCodeSHTypeInstruction() const
{
  InstructionMnemonic functionCode = InstructionMnemonic::INVALID;
  switch (getOp2()) {
    case 0b00:
      functionCode = InstructionMnemonic::L_SLLI;
      break;
    case 0b01:
      functionCode = InstructionMnemonic::L_SRLI;
      break;
    case 0b10:
      functionCode = InstructionMnemonic::L_SRAI;
      break;
    case 0b11:
      functionCode = InstructionMnemonic::L_RORI;
      break;
  }
  return functionCode;
}

InstructionMnemonic 
InstructionDecoder::getFunctionCodeJTypeInstruction() const
{
  InstructionMnemonic functionCode = InstructionMnemonic::INVALID;
  switch(getOpcode()) {
    case 0x00:
      functionCode = InstructionMnemonic::L_J;
      break;
    case 0x01:
      functionCode = InstructionMnemonic::L_JAL;
      break;
    case 0x03:
      functionCode = InstructionMnemonic::L_BNF;
      break;
    case 0x04:
      functionCode = InstructionMnemonic::L_BF;
      break;
  }
  return functionCode;
}

InstructionMnemonic 
InstructionDecoder::getFunctionCodeFTypeInstruction() const
{
  InstructionMnemonic functionCode = InstructionMnemonic::INVALID;
  switch(getOp2()) {
    case 0b00000:
      functionCode = InstructionMnemonic::L_SFEQI;
      break;
    case 0b00001:
      functionCode = InstructionMnemonic::L_SFNEI;
      break;
    case 0b00010:
      functionCode = InstructionMnemonic::L_SFGTUI;
      break;
    case 0b00011:
      functionCode = InstructionMnemonic::L_SFGEUI;
      break;
    case 0b00100:
      functionCode = InstructionMnemonic::L_SFLTUI;
      break;
    case 0b00101:
      functionCode = InstructionMnemonic::L_SFLEUI;
      break;
    case 0b01010:
      functionCode = InstructionMnemonic::L_SFGTSI;
      break;
    case 0b01011:
      functionCode = InstructionMnemonic::L_SFGESI;
      break;
    case 0b01100:
      functionCode = InstructionMnemonic::L_SFLTSI;
      break;
    case 0b01101:
      functionCode = InstructionMnemonic::L_SFLESI;
      break;
  }
  return functionCode;
}

InstructionMnemonic 
InstructionDecoder::getFunctionCodeDNTypeInstruction() const
{
  return InstructionMnemonic::L_ADRP;
}

InstructionMnemonic 
InstructionDecoder::getFunctionCodeORKTypeInstruction() const
{
  return InstructionMnemonic::L_NOP;
}

InstructionMnemonic 
InstructionDecoder::getFunctionCodeDROKTypeInstruction() const
{
  InstructionMnemonic functionCode = InstructionMnemonic::INVALID;
  if (getOp2())
    functionCode = InstructionMnemonic::L_MACRC;
  else
    functionCode = InstructionMnemonic::L_MOVHI;
  return functionCode;
}

InstructionMnemonic 
InstructionDecoder::getFunctionCodeOKTypeInstruction() const
{
  InstructionMnemonic functionCode = InstructionMnemonic::INVALID;
  switch (getOp2()) {
    case 0b0000000000: 
      functionCode = InstructionMnemonic::L_SYS;
      break;
    case 0b0100000000: 
      functionCode = InstructionMnemonic::L_TRAP;
      break;
    case 0b10000000000000000000000000: 
      functionCode = InstructionMnemonic::L_MSYNC;
      break;
    case 0b10100000000000000000000000: 
      functionCode = InstructionMnemonic::L_PSYNC;
      break;
    case 0b11000000000000000000000000: 
      functionCode = InstructionMnemonic::L_CSYNC;
      break;
  }
  return functionCode;
}

InstructionMnemonic 
InstructionDecoder::getFunctionCodeRESTypeInstruction() const
{
  InstructionMnemonic functionCode = InstructionMnemonic::INVALID;
  switch(getOpcode()) {
    case 0x09:
      functionCode = InstructionMnemonic::L_RFE;
      break;
    case 0x1C:
      functionCode = InstructionMnemonic::L_CUST1;
      break;
    case 0x1D:
      functionCode = InstructionMnemonic::L_CUST2;
      break;
    case 0x1E:
      functionCode = InstructionMnemonic::L_CUST3;
      break;
    case 0x1F:
      functionCode = InstructionMnemonic::L_CUST4;
      break;
    case 0x3D:
      functionCode = InstructionMnemonic::L_CUST6;
      break;
    case 0x3E:
      functionCode = InstructionMnemonic::L_CUST7;
      break;
    case 0x3F:
      functionCode = InstructionMnemonic::L_CUST8;
      break;
  }
  return functionCode;
}

// InstructionMnemonic 
// InstructionDecoder::getFunctionCodeDABROOTypeInstruction()
// {
//  return InstructionMnemonic::L_ADD;

// }

InstructionMnemonic 
InstructionDecoder::getFunctionCodeRBRTypeInstruction() const
{
  InstructionMnemonic functionCode = InstructionMnemonic::INVALID;
  switch(getOpcode()) {
    case 0x11:
      functionCode = InstructionMnemonic::L_JR;
      break;
    case 0x12:
      functionCode = InstructionMnemonic::L_JALR;
      break;
  }
  return functionCode;
}

InstructionMnemonic 
InstructionDecoder::getFunctionCodeRAITypeInstruction() const
{
  return InstructionMnemonic::L_MACI;
}

InstructionMnemonic 
InstructionDecoder::getFunctionCodeDAKTypeInstruction() const
{
  InstructionMnemonic functionCode = InstructionMnemonic::INVALID;
  switch(getOpcode()) {
    case 0x29:
      functionCode = InstructionMnemonic::L_ANDI;
      break;
    case 0x2A:
      functionCode = InstructionMnemonic::L_ORI;
      break;
    case 0x2D:
      functionCode = InstructionMnemonic::L_MFSPR;
      break;
  }
  return functionCode;
}

InstructionMnemonic 
InstructionDecoder::getFunctionCodeKABKTypeInstruction() const
{
  return InstructionMnemonic::L_MTSPR;
}

InstructionMnemonic 
InstructionDecoder::getFunctionCodeRABROTypeInstruction() const
{
  InstructionMnemonic functionCode = InstructionMnemonic::INVALID;
  switch(getOp2()) {
    case 0b0000:
      functionCode = InstructionMnemonic::L_MAC;
      break;
    case 0b0001:
      functionCode = InstructionMnemonic::L_MACU;
      break;
    case 0b0010:
      functionCode = InstructionMnemonic::L_MSB;
      break;
    case 0b0011:
      functionCode = InstructionMnemonic::L_MSBU;
      break;
  }
  return functionCode;
}

InstructionMnemonic 
InstructionDecoder::getFunctionCodeOABRTypeInstruction() const
{
  InstructionMnemonic functionCode = InstructionMnemonic::INVALID;
  switch(getOp2()) {
    case 0b00000:
      functionCode = InstructionMnemonic::L_SFEQ;
      break;
    case 0b00001:
      functionCode = InstructionMnemonic::L_SFNE;
      break;
    case 0b00010:
      functionCode = InstructionMnemonic::L_SFGTU;
      break;
    case 0b00011:
      functionCode = InstructionMnemonic::L_SFGEU;
      break;
    case 0b00100:
      functionCode = InstructionMnemonic::L_SFLTU;
      break;
    case 0b00101:
      functionCode = InstructionMnemonic::L_SFLEU;
      break;
    case 0b01010:
      functionCode = InstructionMnemonic::L_SFGTS;
      break;
    case 0b01011:
      functionCode = InstructionMnemonic::L_SFGES;
      break;
    case 0b01100:
      functionCode = InstructionMnemonic::L_SFLTS;
      break;
    case 0b01101:
      functionCode = InstructionMnemonic::L_SFLES;
      break;
  }
  return functionCode;
}

InstructionMnemonic 
InstructionDecoder::getFunctionCodeDABLKTypeInstruction() const
{
  return InstructionMnemonic::L_CUST5;
}