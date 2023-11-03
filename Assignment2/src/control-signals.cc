#include "control-signals.h"
#include <iostream>


ControlSignals::ControlSignals()
  : opCode(), functionCode() 
{
}

InputSelectorEXStage ControlSignals::AInput() const
{
  InputSelectorEXStage input = InputSelectorEXStage::InputTwo;
  if (isBranch()) {
    input = InputSelectorEXStage::InputOne;
  }
  return input;
}

InputSelectorEXStage ControlSignals::BInput() const
{
  InputSelectorEXStage input = InputSelectorEXStage::InputOne;
  switch (opCode) {
    case 0x00:
    case 0x01:
    case 0x03:
    case 0x04:
    case 0x02:
    case 0x05:
    case 0x06:
    case 0x08:
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
    case 0x29:
    case 0x2A:
    case 0x2D:
    case 0x2E:
    case 0x2F:
    case 0x30:
    case 0x33:
    case 0x35:
    case 0x36:
    case 0x37:
      // instruction contains an immeidate
      input = InputSelectorEXStage::InputTwo;
      break;
    default:
      // instruction does not have an immediate so we stay on second input
      break;
  }
  return input;
}

ALUOp ControlSignals::ALUOp() const
{

  switch (this->functionCode) {
    case InstructionMnemonic::L_ADDI:
    case InstructionMnemonic::L_ADDIC:
    case InstructionMnemonic::L_ADDC:
    case InstructionMnemonic::L_ADD:
      // Perform ALU operation ADD
      return ALUOp::ADD;
      break;
    case InstructionMnemonic::L_SUB:
      // Perform ALU operation SUB
      return ALUOp::SUB;
      break;
    case InstructionMnemonic::L_MULI:
    case InstructionMnemonic::L_MUL:
    case InstructionMnemonic::L_MULU:
    case InstructionMnemonic::L_MULD:
    case InstructionMnemonic::L_MULDU:
    case InstructionMnemonic::L_MAC:
    case InstructionMnemonic::L_MACU:
    case InstructionMnemonic::L_MACI:
    case InstructionMnemonic::L_MSB:
    case InstructionMnemonic::L_MSBU:
      // Perform ALU operation MUL
      return ALUOp::MUL;
      break;
    case InstructionMnemonic::L_DIV:
    case InstructionMnemonic::L_DIVU:
      // Perform ALU operation DIV
      return ALUOp::DIV;
      break;
    case InstructionMnemonic::L_ANDI:
    case InstructionMnemonic::L_AND:
      // Perform ALU operation AND
      return ALUOp::AND;
      break;
    case InstructionMnemonic::L_ORI:
    case InstructionMnemonic::L_OR:
      // Perform ALU operation OR
      return ALUOp::OR;
      break;
    case InstructionMnemonic::L_XORI:
    case InstructionMnemonic::L_XOR:
      // Perform ALU operation XOR
      return ALUOp::XOR;
      break;
    case InstructionMnemonic::L_SLLI:
    case InstructionMnemonic::L_SLL:
      // Perform ALU operation SHL
      return ALUOp::SHL;
      break;
    case InstructionMnemonic::L_SRLI:
    case InstructionMnemonic::L_SRL:
      // Perform ALU operation SHR
      return ALUOp::SHR;
      break;
    case InstructionMnemonic::L_SRAI:
    case InstructionMnemonic::L_SRA:
      // Perform ALU operation SAR
      return ALUOp::SAR;
      break;
    case InstructionMnemonic::L_SFEQI:
    case InstructionMnemonic::L_SFEQ:
      // Perform ALU operation EQ
      return ALUOp::EQ;
      break;
    case InstructionMnemonic::L_SFNEI:
    case InstructionMnemonic::L_SFNE:
      // Perform ALU operation NEQ
      return ALUOp::NEQ;
      break;
    case InstructionMnemonic::L_SFGTUI:
    case InstructionMnemonic::L_SFGTSI:
    case InstructionMnemonic::L_SFGTU:
    case InstructionMnemonic::L_SFGTS:
      // Perform ALU operation LT
      return ALUOp::GT;
      break;
    case InstructionMnemonic::L_SFGEUI:
    case InstructionMnemonic::L_SFGESI:
    case InstructionMnemonic::L_SFGEU:
    case InstructionMnemonic::L_SFGES:
      // Perform ALU operation LTE
      return ALUOp::GTE;
      break;
    case InstructionMnemonic::L_SFLTUI:
    case InstructionMnemonic::L_SFLTSI:
      // Perform ALU operation LTE
      return ALUOp::LT;
      break;
    case InstructionMnemonic::L_SFLEUI:
    case InstructionMnemonic::L_SFLESI:
      // Perform ALU operation LTE
      return ALUOp::LTE;
      break;
    case InstructionMnemonic::L_NOP:
      // Perform ALU operation NOP
      return ALUOp::NOP;
      break;

    case InstructionMnemonic::INVALID:
      // Handle MISC instruction
      break;
    default:
      // Handle other cases or throw an exception
      std::cerr << "ALU operation not supported" << std::endl;
      break;
  }

}
    

bool ControlSignals::regWriteInput() const
{
    return false;
}

bool ControlSignals::isBranch() const
{
  switch (this->functionCode) {
    case InstructionMnemonic::L_J:
    case InstructionMnemonic::L_JAL:
    case InstructionMnemonic::L_BNF:
    case InstructionMnemonic::L_BF:
    case InstructionMnemonic::L_RFE:
    case InstructionMnemonic::L_JR:
    case InstructionMnemonic::L_JALR:
      // Handle branch instruction
      return true;
      break; 
    default:
      // we are not dealing with a branch instruction
      return false;
      break;
  }
}
