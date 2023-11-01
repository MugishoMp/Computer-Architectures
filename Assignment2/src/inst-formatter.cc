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
#include <sstream>


void printDNTypeInstruction(std::ostream &os, const InstructionDecoder &decoder){
  // 0x02
  os << "l.adrp" << " ";
  os << "r" << int(decoder.getD()) << ", ";
  os << int(decoder.getImmediateI()) << " ";
}
void printORKTypeInstruction(std::ostream &os, const InstructionDecoder &decoder){
  // 0x05
  os << "l.nop" << " ";
  
  // Create a stringstream
  std::stringstream ss;

  // Convert the integer to a hexadecimal string
  ss << std::hex << int(decoder.getK());

  // Get the hexadecimal string from the stringstream
  std::string hexString = ss.str();

  os << "0x" << hexString << " ";
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
  if (decoder.getFunctionCode() == InstructionMnemonic::L_MACRC)
    os << "l.macrc" << " ";
  else
    os << "l.movhi" << " ";

  os << "r" << int(decoder.getD()) ;

  if (decoder.getFunctionCode() == InstructionMnemonic::L_MOVHI) {
    
    // Create a stringstream
    std::stringstream ss;

    // Convert the integer to a hexadecimal string
    ss << std::hex << int(decoder.getK());

    // Get the hexadecimal string from the stringstream
    std::string hexString = ss.str();

    os << "0x" << hexString << " ";
  }
}
void printRBRTypeInstruction(std::ostream &os, const InstructionDecoder &decoder){
  switch(decoder.getFunctionCode()) {
    case InstructionMnemonic::L_JR:
      os << "l.jr" << " ";
      break;
    case InstructionMnemonic::L_JALR:
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
  switch(decoder.getFunctionCode()) {
    case InstructionMnemonic::L_ANDI:
      os << "l.andi" << " ";
      break;
    case InstructionMnemonic::L_ORI:
      os << "l.ori" << " ";
      break;
    case InstructionMnemonic::L_MFSPR:
      os << "l.mfspr" << " ";
      break;
  }
  os << "r" << int(decoder.getD()) << ", ";
  os << "r" << int(decoder.getA()) << ", ";

  // Create a stringstream
  std::stringstream ss;

  // Convert the integer to a hexadecimal string
  ss << std::hex << int(decoder.getK());

  // Get the hexadecimal string from the stringstream
  std::string hexString = ss.str();

  os << "0x" << hexString << " ";
}
void printSTypeInstruction(std::ostream &os, const InstructionDecoder &decoder){
  switch(decoder.getFunctionCode()) {
    case InstructionMnemonic::L_SWA:
      os << "l.swa" << " ";
      break;
    case InstructionMnemonic::L_SW:
      os << "l.sw" << " ";
      break;
    case InstructionMnemonic::L_SB:
      os << "l.sb" << " ";
      break;
    case InstructionMnemonic::L_SH:
      os << "l.sh" << " ";
      break;
  }
  os << decoder.getImmediateI() << "(" << "r" << int(decoder.getA()) << ")" << ", ";
  os << "r" << int(decoder.getB()) << " ";
}
void printRABROTypeInstruction(std::ostream &os, const InstructionDecoder &decoder){
  // 0x31
  switch(decoder.getFunctionCode()) {
    case InstructionMnemonic::L_MAC:
      os << "l.mac" << " ";
      break;
    case InstructionMnemonic::L_MACU:
      os << "l.macu" << " ";
      break;
    case InstructionMnemonic::L_MSB:
      os << "l.msb" << " ";
      break;
    case InstructionMnemonic::L_MSBU:
      os << "l.msbu" << " ";
      break;
  }
  os << "r" << int(decoder.getA()) << ", ";
  os << "r" << int(decoder.getB()) << " ";
}
void printSHTypeInstruction(std::ostream &os, const InstructionDecoder &decoder){
  // 0x2E
  switch (decoder.getFunctionCode()) {
    case InstructionMnemonic::L_SLLI:
      os << "l.slli" << " ";
      break;
    case InstructionMnemonic::L_SRLI:
      os << "l.srli" << " ";
      break;
    case InstructionMnemonic::L_SRAI:
      os << "l.srai" << " ";
      break;
    case InstructionMnemonic::L_RORI:
      os << "l.rori" << " ";
      break;
  }
  os << "r" << int(decoder.getD()) << ", ";
  os << "r" << int(decoder.getA()) << ", ";
  os << int(decoder.getL()) << " ";
}
void printJTypeInstruction(std::ostream &os, const InstructionDecoder &decoder){
  switch(decoder.getFunctionCode()) {
    case InstructionMnemonic::L_J:
      os << "l.j" << " ";
      break;
    case InstructionMnemonic::L_JAL:
      os << "l.jal" << " ";
      break;
    case InstructionMnemonic::L_BNF:
      os << "l.bnf" << " ";
      break;
    case InstructionMnemonic::L_BF:
      os << "l.bf" << " ";
      break;
  }

  // how do i know what the instruction address is?
  // os << isntructionAddresss + (decoder.getImmediateN() << 2) << " ";
  os << "(" << "<current instruction address> + " << decoder.getImmediateN() << ")" << " ";
}
void printOKTypeInstruction(std::ostream &os, const InstructionDecoder &decoder){
  // 0x08
  switch (decoder.getFunctionCode()) {
    case InstructionMnemonic::L_SYS: 
      os << "l.sys" << " ";
      os << int(decoder.getK()) << " ";
      break;
    case InstructionMnemonic::L_TRAP: 
      os << "l.trap" << " ";
      os << int(decoder.getK()) << " ";
      break;
    case InstructionMnemonic::L_MSYNC: 
      os << "l.msync" << " ";
      break;
    case InstructionMnemonic::L_PSYNC: 
      os << "l.psync" << " ";
      break;
    case InstructionMnemonic::L_CSYNC: 
      os << "l.csync" << " ";
      break;
  }
}
void printRESTypeInstruction(std::ostream &os, const InstructionDecoder &decoder){
  switch(decoder.getFunctionCode()) {
    case InstructionMnemonic::L_RFE:
      os << "l.rfe" << " ";
      break;
    case InstructionMnemonic::L_CUST1:
      os << "l.cust1" << " ";
      break;
    case InstructionMnemonic::L_CUST2:
      os << "l.cust2" << " ";
      break;
    case InstructionMnemonic::L_CUST3:
      os << "l.cust3" << " ";
      break;
    case InstructionMnemonic::L_CUST4:
      os << "l.cust4" << " ";
      break;
    case InstructionMnemonic::L_CUST6:
      os << "l.cust6" << " ";
      break;
    case InstructionMnemonic::L_CUST7:
      os << "l.cust7" << " ";
      break;
    case InstructionMnemonic::L_CUST8:
      os << "l.cust8" << " ";
      break;
  }
}
void printFTypeInstruction(std::ostream &os, const InstructionDecoder &decoder){
  // 0x2F
  switch(decoder.getFunctionCode()) {
    case InstructionMnemonic::L_SFEQI:
      os << "l.sfeqi" << " ";
      break;
    case InstructionMnemonic::L_SFNEI:
      os << "l.sfnei" << " ";
      break;
    case InstructionMnemonic::L_SFGTUI:
      os << "l.sfgtui" << " ";
      break;
    case InstructionMnemonic::L_SFGEUI:
      os << "l.sfgeui" << " ";
      break;
    case InstructionMnemonic::L_SFLTUI:
      os << "l.sfltui" << " ";
      break;
    case InstructionMnemonic::L_SFLEUI:
      os << "l.sfleui" << " ";
      break;
    case InstructionMnemonic::L_SFGTSI:
      os << "l.sfgtsi" << " ";
      break;
    case InstructionMnemonic::L_SFGESI:
      os << "l.sfgesi" << " ";
      break;
    case InstructionMnemonic::L_SFLTSI:
      os << "l.sfltsi" << " ";
      break;
    case InstructionMnemonic::L_SFLESI:
      os << "l.sflesi" << " ";
      break;
  }
  os << "r" << int(decoder.getA()) << ", ";
  os << int(decoder.getImmediateI()) << " ";
}
void printOABRTypeInstruction(std::ostream &os, const InstructionDecoder &decoder){
  // 0x39
  switch(decoder.getFunctionCode()) {
    case InstructionMnemonic::L_SFEQ:
      os << "l.sfeq" << " ";
      break;
    case InstructionMnemonic::L_SFNE:
      os << "l.sfne" << " ";
      break;
    case InstructionMnemonic::L_SFGTU:
      os << "l.sfgtu" << " ";
      break;
    case InstructionMnemonic::L_SFGEU:
      os << "l.sfgeu" << " ";
      break;
    case InstructionMnemonic::L_SFLTU:
      os << "l.sfltu" << " ";
      break;
    case InstructionMnemonic::L_SFLEU:
      os << "l.sfleu" << " ";
      break;
    case InstructionMnemonic::L_SFGTS:
      os << "l.sfgts" << " ";
      break;
    case InstructionMnemonic::L_SFGES:
      os << "l.sfges" << " ";
      break;
    case InstructionMnemonic::L_SFLTS:
      os << "l.sflts" << " ";
      break;
    case InstructionMnemonic::L_SFLES:
      os << "l.sfles" << " ";
      break;
  }
  os << "r" << int(decoder.getA()) << ", ";
  os << "r" << int(decoder.getB()) << " ";
}

void printITypeInstruction(std::ostream &os, const InstructionDecoder &decoder){
  switch(decoder.getFunctionCode()) {
    case InstructionMnemonic::L_LF:
      os << "l.lf" << " ";
      break;
    case InstructionMnemonic::L_LWA:
      os << "l.lwa" << " ";
      break;
    case InstructionMnemonic::L_LD:
      os << "l.ld" << " ";
      break;
    case InstructionMnemonic::L_LWZ:
      os << "l.lwz" << " ";
      break;
    case InstructionMnemonic::L_LWS:
      os << "l.lws" << " ";
      break;
    case InstructionMnemonic::L_LBZ:
      os << "l.lbz" << " ";
      break;
    case InstructionMnemonic::L_LBS:
      os << "l.lbs" << " ";
      break;
    case InstructionMnemonic::L_LHZ:
      os << "l.lhz" << " ";
      break;
    case InstructionMnemonic::L_LHS:
      os << "l.lhs" << " ";
      break;
    case InstructionMnemonic::L_ADDI:
      os << "l.addi" << " ";
      break;
    case InstructionMnemonic::L_ADDIC:
      os << "l.addic" << " ";
      break;
    case InstructionMnemonic::L_XORI:
      os << "l.xori" << " ";
      break;
    case InstructionMnemonic::L_MULI:
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
  switch(decoder.getFunctionCode()) {
    case InstructionMnemonic::L_ADD:
      os << "l.add" << " ";
      break;
    case InstructionMnemonic::L_ADDC:
      os << "l.addc" << " ";
      break;
    case InstructionMnemonic::L_SUB:
      os << "l.sub" << " ";
      break;
    case InstructionMnemonic::L_AND:
      os << "l.and" << " ";
      break;
    case InstructionMnemonic::L_OR:
      os << "l.or" << " ";
      break;
    case InstructionMnemonic::L_XOR:
      os << "l.xor" << " ";
      break;
    case InstructionMnemonic::L_MUL:
      os << "l.mul" << " ";
      break;
    case InstructionMnemonic::L_MULD:
      os << "l.muld" << " ";
      break;
    case InstructionMnemonic::L_SLL:
      os << "l.sll" << " ";
      break;
    case InstructionMnemonic::L_SRL:
      os << "l.srl" << " ";
      break;
    case InstructionMnemonic::L_SRA:
      os << "l.sra" << " ";
      break;
    case InstructionMnemonic::L_ROR:
      os << "l.ror" << " ";
      break;
    case InstructionMnemonic::L_DIV:
      os << "l.div" << " ";
      break;
    case InstructionMnemonic::L_DIVU:
      os << "l.divu" << " ";
      break;
    case InstructionMnemonic::L_MULU:
      os << "l.mulu" << " ";
      break;
    case InstructionMnemonic::L_EXTHS:
      os << "l.exths" << " ";
      break;
    case InstructionMnemonic::L_EXTBS:
      os << "l.extbs" << " ";
      break;
    case InstructionMnemonic::L_EXTHZ:
      os << "l.exthz" << " ";
      break;
    case InstructionMnemonic::L_EXTBZ:
      os << "l.extbz" << " ";
      break;
    case InstructionMnemonic::L_MULDU:
      os << "l.muldu" << " ";
      break;
    case InstructionMnemonic::L_EXTWS:
      os << "l.extws" << " ";
      break;
    case InstructionMnemonic::L_EXTWZ:
      os << "l.extwz" << " "; 
      break;
    case InstructionMnemonic::L_CMOV:
      os << "l.cmov" << " ";
      break;
    case InstructionMnemonic::L_FF1:
      os << "l.ff1" << " ";
      break;
    case InstructionMnemonic::L_FL1:
      os << "l.fl1" << " ";
      break;

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
