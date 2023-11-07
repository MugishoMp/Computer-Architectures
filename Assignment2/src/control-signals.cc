#include "control-signals.h"
#include <iostream>


ControlSignals::ControlSignals()
  : opCode(), functionCode() 
{
}

InputSelectorEXStage ControlSignals::AInput() const
{
  // we basically check if the instruction is an branch instruction. If it is
  // we take the mux entry that contains the PC
  InputSelectorEXStage input = InputSelectorEXStage::InputTwo;
  if (isBranch()) {
    input = InputSelectorEXStage::InputOne;
  }
  return input;
}

InputSelectorEXStage ControlSignals::BInput() const
{
  // we basically check if the instruction has an immediate field or not.
  // if it has we will pick the mux entry that contains the immediate
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

ALUOp ControlSignals::AluOp() const
{

  // std::cerr << "function code in AluOp: " << static_cast<int>(this->functionCode) << std::endl;

  switch (this->functionCode) {
    case InstructionMnemonic::L_JR:
      // Perform ALU operation ADD
      return ALUOp::B;
      break;

      
    case InstructionMnemonic::L_MOVHI:
      return ALUOp::SHL_16;
      break;

    case InstructionMnemonic::L_ADDI:
    case InstructionMnemonic::L_ADD:
    case InstructionMnemonic::L_LWZ:
    case InstructionMnemonic::L_SW:
    case InstructionMnemonic::L_LBZ:
    case InstructionMnemonic::L_SB:
      // Perform ALU operation ADD
      return ALUOp::ADD;
      break;

    case InstructionMnemonic::L_JAL:
    case InstructionMnemonic::L_J:
    case InstructionMnemonic::L_BF:
    case InstructionMnemonic::L_BNF:
      // Perform ALU operation ADD
      return ALUOp::SHIFT_ADD;
      break;

    case InstructionMnemonic::L_SUB:
      // Perform ALU operation SUB
      return ALUOp::SUB;
      break;
    case InstructionMnemonic::L_MACI:
      // Perform ALU operation MUL
      return ALUOp::MUL;
      break;
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
    case InstructionMnemonic::L_SRA:
      // Perform ALU operation SHR
      return ALUOp::SHR;
      break;
    case InstructionMnemonic::L_SFEQI:
    case InstructionMnemonic::L_SFEQ:
      // Perform ALU operation EQ
      return ALUOp::EQ;
      break;
    case InstructionMnemonic::L_SFNE:
      // Perform ALU operation NEQ
      return ALUOp::NEQ;
      break;
    case InstructionMnemonic::L_SFGTU:
    case InstructionMnemonic::L_SFGTS:
      // Perform ALU operation LT
      return ALUOp::GT;
      break;
    case InstructionMnemonic::L_SFGES:
      // Perform ALU operation LTE
      return ALUOp::GTE;
      break;
    case InstructionMnemonic::L_SFLES:
      return ALUOp::LTE;
      break;
    case InstructionMnemonic::L_NOP:
      // Perform ALU operation NOP
      return ALUOp::NOP;
      break;

    default:
      // Handle other cases or throw an exception
      std::cerr << "ALU operation not supported: " << static_cast<int>(this->functionCode)<< std::endl;
      break;
  }

}


bool ControlSignals::regWriteInput() const
{
  switch(this->functionCode) {
    // Arithmetic and Logical Operations
    case InstructionMnemonic::L_ADD:
    case InstructionMnemonic::L_SUB:
    case InstructionMnemonic::L_OR:
    case InstructionMnemonic::L_SLL:
    case InstructionMnemonic::L_SRA:

    // Load Instructions
    case InstructionMnemonic::L_LWZ:
    case InstructionMnemonic::L_LBZ:
    case InstructionMnemonic::L_LBS:

    // Immediate Instructions
    case InstructionMnemonic::L_ADDI:
    case InstructionMnemonic::L_ANDI:
    case InstructionMnemonic::L_ORI:
    case InstructionMnemonic::L_SLLI:
    case InstructionMnemonic::L_SRLI:
    case InstructionMnemonic::L_MOVHI:
      return true;


    // Jump and Link Instructions
    case InstructionMnemonic::L_JAL:
      return true;

    case InstructionMnemonic::L_AND:
    case InstructionMnemonic::L_SRL:
      std::cerr << "regWriteInput::instruction not implemented" << std::endl;
    // For all other instructions
    default:
      return false;
  }
}

InputSelectorWBStage ControlSignals::isReadOp() const 
{
  switch (this->functionCode) {
    case InstructionMnemonic::L_LWA:
    case InstructionMnemonic::L_LWZ:
    case InstructionMnemonic::L_LBZ:
    case InstructionMnemonic::L_LBS:
      return InputSelectorWBStage::InputOne;
    default:
      return InputSelectorWBStage::InputTwo;
  }

}


bool ControlSignals::isReadOpBool() const 
{
  switch (this->functionCode) {
    case InstructionMnemonic::L_LWA:
    case InstructionMnemonic::L_LWZ:
    case InstructionMnemonic::L_LBZ:
    case InstructionMnemonic::L_LBS:
      return true;
    default:
      return false;
  }
}

bool ControlSignals::isWriteOpBool() const 
{
  switch (this->functionCode) {
    case InstructionMnemonic::L_SW:
    case InstructionMnemonic::L_SB:
    case InstructionMnemonic::L_SH:
      return true;
    default:
      return false;
  }
}

uint8_t ControlSignals::getDataSize() const 
{
  switch (this->functionCode) {
    // Word-sized operations (4 bytes)
    case InstructionMnemonic::L_LWA:
    case InstructionMnemonic::L_SW:
    case InstructionMnemonic::L_LWZ:
      return 4;

    // Half-word-sized operations (2 bytes)
    case InstructionMnemonic::L_SH:
      return 2;

    // Byte-sized operations (1 byte)
    case InstructionMnemonic::L_LBZ:
    case InstructionMnemonic::L_LBS:
    case InstructionMnemonic::L_SB:
      return 1;
    // Default case for other instructions
    default:
      return 0;
  }
}

bool ControlSignals::isBranch() const
{
  switch (this->functionCode) {
    case InstructionMnemonic::L_J:
    case InstructionMnemonic::L_JAL:
    case InstructionMnemonic::L_BNF:
    case InstructionMnemonic::L_BF:
    case InstructionMnemonic::L_JR:
      // Handle branch instruction
      return true;
      break; 
    default:
      // we are not dealing with a branch instruction
      return false;
      break;
  }
}

bool ControlSignals::jump(bool FLAG) const
{
  switch (this->functionCode) {
    case InstructionMnemonic::L_J:
      // PC ← exts(Immediate << 2) + JumpInsnAd
      return true;
    case InstructionMnemonic::L_JAL:
      // PC ← exts(Immediate << 2) + JumpInsnAddr
      // LR ← CPUCFGR[ND] ? JumpInsnAddr + 4 : DelayInsnAddr + 4
      return true;
    case InstructionMnemonic::L_JR:
      // PC ← rB
      return true;
      break; 
    case InstructionMnemonic::L_BF:
      // EA ← exts(Immediate << 2) + BranchInsnAddr
      // PC ← EA if SR[F] set
      return FLAG;
    case InstructionMnemonic::L_BNF:
      // EA ← exts(Immediate << 2) + BranchInsnAddr
      // PC ← EA if SR[F] cleared
      return !FLAG;
    default:
      // we are not dealing with a branch instruction
      return false;
      break;
  }
}

bool ControlSignals::setLinkRegister() const 
{
  switch (this->functionCode) {
    case InstructionMnemonic::L_JAL:
      return true;
    default:
      return false;
  }
}
