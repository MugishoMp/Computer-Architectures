/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    inst-formatter.cc - RISC-V instruction printer (disassembler)
 *
 * Copyright (C) 2016,2018  Leiden University, The Netherlands.
 */

#include "inst-decoder.h"

#include <functional>
#include <map>
#include <iostream>


void printDNTypeInstruction(std::ostream &os, const InstructionDecoder &decoder){
  // 0x02
  os << "l.adrp" << " ";
  os << "r" << int(decoder.getD()) << ", ";
  os << int(decoder.getImmediateI()) << " ";
}
void printORKTypeInstruction(std::ostream &os, const InstructionDecoder &decoder){
  // 0x05
  os << "l.nop" << " ";
  os << int(decoder.getK()) << " ";
}
void printRAITypeInstruction(std::ostream &os, const InstructionDecoder &decoder){
  // 0x13
  os << "l.maci" << " ";
  os << "r" << int(decoder.getA()) << ", ";
  os << int(decoder.getImmediateI()) << " ";
}
void printKABKTypeInstruction(std::ostream &os, const InstructionDecoder &decoder){
  // 0x30
  os << "l.mtspr" << " ";
  os << "r" << int(decoder.getA()) << ", ";
  os << "r" << int(decoder.getB()) << ", ";
  os << int(decoder.getK()) << " ";
}
void printDABLKTypeInstruction(std::ostream &os, const InstructionDecoder &decoder){
  // 0x3C
  os << "l.cust5" << " ";
  os << "r" << int(decoder.getD()) << ", ";
  os << "r" << int(decoder.getA()) << ", ";
  os << "r" << int(decoder.getA()) << ", ";
  os << int(decoder.getL()) << ", ";
  os << int(decoder.getK()) << " ";
}
void printDROKTypeInstruction(std::ostream &os, const InstructionDecoder &decoder){
  // 0x06
  if (decoder.getOp2())
    os << "l.macrc" << " ";
  else
    os << "l.movhi" << " ";

  os << "r" << int(decoder.getD()) ;

  if (decoder.getOp2())
    os << ", " << int(decoder.getK()) << " ";
}
void printRBRTypeInstruction(std::ostream &os, const InstructionDecoder &decoder){
  switch(decoder.getOpcode()) {
    case 0x11:
      os << "l.jr" << " ";
      break;
    case 0x12:
      os << "l.jalr" << " ";
      break;
  }
  os << "r" << int(decoder.getB()) << " ";
}
// void printDABROOTypeInstruction(std::ostream &os, const InstructionDecoder &decoder){
//   switch(decoder.getOpcode()) {
//     case 0x0A:
//       os << "_._" << " ";
//       break;
//     case 0x32:
//       os << "_._" << " ";
//       break;
//   }
//   os << "r" << int(decoder.getD()) << ", ";
//   os << "r" << int(decoder.getA()) << ", ";
//   os << "r" << int(decoder.getB()) << " ";
// }
void printDAKTypeInstruction(std::ostream &os, const InstructionDecoder &decoder){
  switch(decoder.getOpcode()) {
    case 0x29:
      os << "l.andi" << " ";
      break;
    case 0x2A:
      os << "l.ori" << " ";
      break;
    case 0x2D:
      os << "l.mfspr" << " ";
      break;
  }
  os << "r" << int(decoder.getD()) << ", ";
  os << "r" << int(decoder.getA()) << ", ";
  os << int(decoder.getK()) << " ";
}
void printSTypeInstruction(std::ostream &os, const InstructionDecoder &decoder){
  switch(decoder.getOpcode()) {
    case 0x33:
      os << "l.swa" << " ";
      break;
    case 0x35:
      os << "l.sw" << " ";
      break;
    case 0x36:
      os << "l.sb" << " ";
      break;
    case 0x37:
      os << "l.sh" << " ";
      break;
  }
  os << decoder.getImmediateI() << "(" << "r" << int(decoder.getA()) << ")" << ", ";
  os << "r" << int(decoder.getB()) << " ";
}
void printRABROTypeInstruction(std::ostream &os, const InstructionDecoder &decoder){
  // 0x31
  switch(decoder.getOp2()) {
    case 0b0000:
      os << "l.mac" << " ";
      break;
    case 0b0001:
      os << "l.macu" << " ";
      break;
    case 0b0010:
      os << "l.msb" << " ";
      break;
    case 0b0011:
      os << "l.msbu" << " ";
      break;
  }
  os << "r" << int(decoder.getA()) << ", ";
  os << "r" << int(decoder.getB()) << " ";
}
void printSHTypeInstruction(std::ostream &os, const InstructionDecoder &decoder){
  // 0x2E
  switch (decoder.getOp2()) {
    case 0b00:
      os << "l.slli" << " ";
      break;
    case 0b01:
      os << "l.srli" << " ";
      break;
    case 0b10:
      os << "l.srai" << " ";
      break;
    case 0b11:
      os << "l.rori" << " ";
      break;
  }
  os << "r" << int(decoder.getD()) << ", ";
  os << "r" << int(decoder.getA()) << ", ";
  os << int(decoder.getL()) << " ";
}
void printJTypeInstruction(std::ostream &os, const InstructionDecoder &decoder){
  switch(decoder.getOpcode()) {
    case 0x00:
      os << "l.j" << " ";
      break;
    case 0x01:
      os << "l.jal" << " ";
      break;
    case 0x03:
      os << "l.bnf" << " ";
      break;
    case 0x04:
      os << "l.bf" << " ";
      break;
  }
  os << uint_(decoder.getImmediateN()) << " ";
}
void printOKTypeInstruction(std::ostream &os, const InstructionDecoder &decoder){
  // 0x08
  switch (decoder.getOp2()) {
    case 0b0000000000: 
      os << "l.sys" << " ";
      os << int(decoder.getK()) << " ";
      break;
    case 0b0100000000: 
      os << "l.trap" << " ";
      os << int(decoder.getK()) << " ";
      break;
    case 0b10000000000000000000000000: 
      os << "l.msync" << " ";
      break;
    case 0b10100000000000000000000000: 
      os << "l.psync" << " ";
      break;
    case 0b11000000000000000000000000: 
      os << "l.csync" << " ";
      break;
  }
}
void printRESTypeInstruction(std::ostream &os, const InstructionDecoder &decoder){
  switch(decoder.getOpcode()) {
    case 0x09:
      os << "l.rfe" << " ";
      break;
    case 0x1C:
      os << "l.cust1" << " ";
      break;
    case 0x1D:
      os << "l.cust2" << " ";
      break;
    case 0x1E:
      os << "l.cust3" << " ";
      break;
    case 0x1F:
      os << "l.cust4" << " ";
      break;
    case 0x3D:
      os << "l.cust6" << " ";
      break;
    case 0x3E:
      os << "l.cust7" << " ";
      break;
    case 0x3F:
      os << "l.cust8" << " ";
      break;
  }
}
void printFTypeInstruction(std::ostream &os, const InstructionDecoder &decoder){
  // 0x2F
  switch(decoder.getOp2()) {
    case 0b00000:
      os << "l.sfeqi" << " ";
      break;
    case 0b00001:
      os << "l.sfnei" << " ";
      break;
    case 0b00010:
      os << "l.sfgtui" << " ";
      break;
    case 0b00011:
      os << "l.sfgeui" << " ";
      break;
    case 0b00100:
      os << "l.sfltui" << " ";
      break;
    case 0b00101:
      os << "l.sfleui" << " ";
      break;
    case 0b01010:
      os << "l.sfgtsi" << " ";
      break;
    case 0b01011:
      os << "l.sfgesi" << " ";
      break;
    case 0b01100:
      os << "l.sfltsi" << " ";
      break;
    case 0b01101:
      os << "l.sflesi" << " ";
      break;
  }
  os << "r" << int(decoder.getA()) << ", ";
  os << int(decoder.getImmediateI()) << " ";
}
void printOABRTypeInstruction(std::ostream &os, const InstructionDecoder &decoder){
  // 0x39
  switch(decoder.getOp2()) {
    case 0b00000:
      os << "l.sfeq" << " ";
      break;
    case 0b00001:
      os << "l.sfne" << " ";
      break;
    case 0b00010:
      os << "l.sfgtu" << " ";
      break;
    case 0b00011:
      os << "l.sfgeu" << " ";
      break;
    case 0b00100:
      os << "l.sfltu" << " ";
      break;
    case 0b00101:
      os << "l.sfleu" << " ";
      break;
    case 0b01010:
      os << "l.sfgts" << " ";
      break;
    case 0b01011:
      os << "l.sfges" << " ";
      break;
    case 0b01100:
      os << "l.sflts" << " ";
      break;
    case 0b01101:
      os << "l.sfles" << " ";
      break;
  }
  os << "r" << int(decoder.getA()) << ", ";
  os << "r" << int(decoder.getB()) << " ";
}

void printITypeInstruction(std::ostream &os, const InstructionDecoder &decoder){
  switch(decoder.getOpcode()) {
    case 0x1A:
      os << "l.lf" << " ";
      break;
    case 0x1B:
      os << "l.lwa" << " ";
      break;
    case 0x20:
      os << "l.ld" << " ";
      break;
    case 0x21:
      os << "l.lwz" << " ";
      break;
    case 0x22:
      os << "l.lws" << " ";
      break;
    case 0x23:
      os << "l.lbz" << " ";
      break;
    case 0x24:
      os << "l.lbs" << " ";
      break;
    case 0x25:
      os << "l.lhz" << " ";
      break;
    case 0x26:
      os << "l.lhs" << " ";
      break;
    case 0x27:
      os << "l.addi" << " ";
      break;
    case 0x28:
      os << "l.addic" << " ";
      break;
    case 0x2B:
      os << "l.xori" << " ";
      break;
    case 0x2C:
      os << "l.muli" << " ";
      break;
  }
  os << "r" << int(decoder.getD()) << ", ";
  if (decoder.getOpcode() <= 0x26)
    os << int(decoder.getImmediateI()) << "(" << "r" << int(decoder.getA()) << ")" << " ";
  else 
    os << "r" << int(decoder.getA()) << ", " << int(decoder.getImmediateI()) << " ";
}

void printRTypeInstruction(std::ostream &os, const InstructionDecoder &decoder){
  // 0x38
  switch(decoder.getOp3()) {
    case 0b0000:
      os << "l.add" << " ";
      break;
    case 0b0001:
      os << "l.addc" << " ";
      break;
    case 0b0010:
      os << "l.sub" << " ";
      break;
    case 0b0011:
      os << "l.and" << " ";
      break;
    case 0b0100:
      os << "l.or" << " ";
      break;
    case 0b0101:
      os << "l.xor" << " ";
      break;
    case 0b0110:
      os << "l.mul" << " ";
      break;
    case 0b0111:
      os << "l.muld" << " ";
      break;
    case 0b1000:
      switch (decoder.getOp2()) {
        case 0b0000:
          os << "l.sll" << " ";
          break;
        case 0b0001:
          os << "l.srl" << " ";
          break;
        case 0b0010:
          os << "l.sra" << " ";
          break;
        case 0b0011:
          os << "l.ror" << " ";
          break;
      }
      break;
    case 0b1001:
      os << "l.div" << " ";
      break;
    case 0b1010:
      os << "l.divu" << " ";
      break;
    case 0b1011:
      os << "l.mulu" << " ";
      break;
    case 0b1100:
      switch (decoder.getOp2()) {
        case 0b0000:
          os << "l.exths" << " ";
          break;
        case 0b0001:
          os << "l.extbs" << " ";
          break;
        case 0b0010:
          os << "l.exthz" << " ";
          break;
        case 0b0011:
          if (!((decoder.getInstructionWord() << 9) | 0b1))
            os << "l.extbz" << " ";
          else
            os << "l.muldu" << " ";
          break;
      }
      break;
    case 0b1101:
      if (decoder.getOp2() == 0b0000)
        os << "l.extws" << " ";
      else
        os << "l.extwz" << " ";
      break;
    case 0b1110:
      os << "l.cmov" << " ";
      break;
    case 0b1111:
      switch (decoder.getOp2()) {
        case 0b00:
          os << "l.ff1" << " ";
          break;
        case 0b01:
          os << "l.fl1" << " ";
          break;
      }

  }
  if (int(decoder.getB()) != FIELD_NOT_AVAILABLE_8_BIT)
    os << "r" << int(decoder.getD()) << ", ";
  if (int(decoder.getB()) != FIELD_NOT_AVAILABLE_8_BIT)
    os << "r" << int(decoder.getA()) << ", ";
  if (int(decoder.getB()) != FIELD_NOT_AVAILABLE_8_BIT)
    os << "r" << int(decoder.getB()) << " ";
}

std::ostream &
operator<<(std::ostream &os, const InstructionDecoder &decoder)
{
  /* TODO: write a textual representation of the decoded instruction
   * in "decoder" to the output stream "os". Do not include a newline.
   * And remove the statement below.
   */

  switch (decoder.getInstructionType()) {
    case R:
      printRTypeInstruction(os, decoder);
      break;
    case I:
      printITypeInstruction(os, decoder);
      break;
    case S:
      printSTypeInstruction(os, decoder);
      break;
    case SH:
      printSHTypeInstruction(os, decoder);
      break;
    case J:
      printJTypeInstruction(os, decoder);
      break;
    case F:
      printFTypeInstruction(os, decoder);
      break;
    case DN:
      printDNTypeInstruction(os, decoder);
      break;
    case ORK:
      printORKTypeInstruction(os, decoder);
      break;
    case DROK:
      printDROKTypeInstruction(os, decoder);
      break;
    case OK:
      printOKTypeInstruction(os, decoder);
      break;
    case RES:
      printRESTypeInstruction(os, decoder);
      break;
    // will take too much time i think  
    // case DABROO:
    //   printDABROOTypeInstruction(os, decoder);
    //   break;
    case RBR:
      printRBRTypeInstruction(os, decoder);
      break;
    case RAI:
      printRAITypeInstruction(os, decoder);
      break;
    case DAK:
      printDAKTypeInstruction(os, decoder);
      break;
    case KABK:
      printKABKTypeInstruction(os, decoder);
      break;
    case RABRO:
      printRABROTypeInstruction(os, decoder);
      break;
    case OABR:
      printOABRTypeInstruction(os, decoder);
      break;
    case DABLK:
      printDABLKTypeInstruction(os, decoder);
      break;
    case INVALID:
    default:
      os << "invalid instruction";
  }
  return os;
}
