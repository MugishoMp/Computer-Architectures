/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    stages.cc - Pipeline stages
 *
 * Copyright (C) 2016-2020  Leiden University, The Netherlands.
 */

#include "stages.h"
#include "mux.h"
#include <iostream>

/*
 * Instruction fetch
 */

void
InstructionFetchStage::propagate()
{
  try 
  {
    if (ex_m.BRANCH_DELAY_SLOT) {
      // INPUT Instruction Memory - PC
      instructionMemory.setAddress(PC);
      instructionMemory.setSize(4);
      PC += 4;
    }
    
    RegValue BRANCH_PC = ex_m.ALU_OUTPUT;
    
    Mux<RegValue, InputSelectorIFStage> mux;
    mux.setInput(InputSelectorIFStage::InputOne, PC );
    mux.setInput(InputSelectorIFStage::InputTwo, BRANCH_PC);
    mux.setSelector(ex_m.BRANCH_DECISION);

    PC = mux.getOutput();

    if (!ex_m.BRANCH_DELAY_SLOT) {
      // INPUT Instruction Memory - PC
      instructionMemory.setAddress(PC);
      instructionMemory.setSize(4);
      PC += 4;
    }
    
    instr = instructionMemory.getValue();
    /* Enable this once you have implemented instruction fetch. */
    if (instr == TestEndMarker)
      throw TestEndMarkerEncountered(PC);

  }
  catch (TestEndMarkerEncountered &e)
  {
    throw;
  }
  catch (std::exception &e)
  {
    throw InstructionFetchFailure(PC);
  }
}

void
InstructionFetchStage::clockPulse()
{
  if(pipelining && HAZARD_DETECTOR.isDataHazard()) {
  }

  // PC
  if_id.PC = PC;
  
  // OUTPUT Instruction Memory
  if_id.INSTRUCTION_WORD = instr;
}





/*
 * Instruction decode
 */

void
dump_instruction(std::ostream &os, const uint32_t instructionWord,
                 const InstructionDecoder &decoder);

void
InstructionDecodeStage::propagate()
{
  // PC
  PC = if_id.PC;

  // decode instruction
  decoder.setInstructionWord(if_id.INSTRUCTION_WORD);

  // set control signals
  CONTROL_SIGNALS.setOpcode(decoder.getOpcode());
  CONTROL_SIGNALS.setFunctionCode(decoder.getFunctionCode());

  try {
    A = decoder.getA();
    regfile.setRS1(A);
  } catch (IllegalInstruction &e) {
    A = (RegNumber)MaxRegs;
    regfile.setRS1(A);
    // the control signal rs1Input needs to return false;
  }

  try {
    B = decoder.getB();
    regfile.setRS2(B);
  } catch (IllegalInstruction &e) {
    B = (RegNumber)MaxRegs;
    regfile.setRS2(B);
    // the control signal rs2Input needs to return false;
  }

  // Registers INPUT 3 - RD (the register we want to write data to)
  // handled in the WB stage
  // Registers INPUT 4 - writeData
  // handled in the WB stage

  // Sign Extend INPUT 1
  try {
    SIGN_EXTENDED_IMMEDIATE = decoder.getImmediate();
  } catch (IllegalInstruction &e) {
    SIGN_EXTENDED_IMMEDIATE = 0;
  }

  /* debug mode: dump decoded instructions to cerr.
   * In case of no pipelining: always dump.
   * In case of pipelining: special case, if the PC == 0x0 (so on the
   * first cycle), don't dump an instruction. This avoids dumping a
   * dummy instruction on the first cycle when ID is effectively running
   * uninitialized.
   */
  if (debugMode && (! pipelining || (pipelining && PC != 0x0)))
    {
      /* Dump program counter & decoded instruction in debug mode */
      auto storeFlags(std::cerr.flags());

      std::cerr << std::hex << std::showbase << PC << "\t";
      std::cerr.setf(storeFlags);

      std::cerr << decoder << std::endl;
    }

}

void InstructionDecodeStage::clockPulse()
{
  if(pipelining && HAZARD_DETECTOR.isDataHazard()) {
  }

  // PC
  id_ex.PC = PC;

  // register numbers of the input registers
  id_ex.A = A;
  id_ex.B = B;

  id_ex.CONTROL_SIGNALS = CONTROL_SIGNALS;

  // Registers OUTPUT 1
  id_ex.RS1 = regfile.getReadData1();
  // Registers OUTPUT 2
  id_ex.RS2 = regfile.getReadData2();

  // Sign Extend OUTPUT 1
  id_ex.IMMEDIATE = SIGN_EXTENDED_IMMEDIATE;

  // Register RD
  try {
    // set rd to 9
    if (CONTROL_SIGNALS.setLinkRegister()) {
      // set link register
      id_ex.RD = 9;
    } else {
      id_ex.RD = decoder.getD();
    }
  } catch (IllegalInstruction &e) {
    id_ex.RD = (RegNumber) MaxRegs;
  }

  /* ignore the "instruction" in the first cycle. */
  if (! pipelining || (pipelining && PC != 0x0))
    ++nInstrIssued;

}




/*
 * Execute
 */

void
ExecuteStage::propagate()
{

  // PC
  PC = id_ex.PC;

  // CONTROL_SIGNALS
  CONTROL_SIGNALS = id_ex.CONTROL_SIGNALS;

  // Mux 1 - input A of the ALU
  Mux<RegValue, InputSelectorEXStage> mux1;
  mux1.setInput(InputSelectorEXStage::InputOne, PC);
  mux1.setInput(InputSelectorEXStage::InputTwo, id_ex.RS1);
  mux1.setSelector(CONTROL_SIGNALS.AInput());
  
  // Mux 2 - input B of the ALU
  Mux<RegValue, InputSelectorEXStage> mux2;
  mux2.setInput(InputSelectorEXStage::InputOne, id_ex.RS2);
  mux2.setInput(InputSelectorEXStage::InputTwo, id_ex.IMMEDIATE);
  mux2.setSelector(CONTROL_SIGNALS.BInput());

  // ALU
  alu.setA(mux1.getOutput());
  alu.setB(mux2.getOutput());
  alu.setOp(CONTROL_SIGNALS.AluOp());

  // setting the flags after ALU operation
  FLAG = alu.getFlag();
  ZERO_FLAG = alu.getZeroFlag();
  SIGN_FLAG = alu.getSignFlag();
  CARRY_FLAG = alu.getCarryFlag();
  OVERFLOW_FLAG = alu.getOverflowFlag();


  // Check to see if we are already in a branch delay slot, if so reset the flag
  if (BRANCH_DELAY_SLOT) {
    BRANCH_DELAY_SLOT = false;
  }

  // Deciding if we need to jump after this instruction
  if (CONTROL_SIGNALS.jump(FLAG)){
    BRANCH_DECISION = InputSelectorIFStage::InputTwo;
    BRANCH_DELAY_SLOT = true;
  } else {
    BRANCH_DECISION = InputSelectorIFStage::InputOne;
  }

  // RS2
  RS2 = id_ex.RS2;

  // RD
  RD = id_ex.RD;

}

void
ExecuteStage::clockPulse()
{
  if(pipelining && HAZARD_DETECTOR.isDataHazard()) {
  }

  // PC
  ex_m.PC = PC;

  // CONTROL_SIGNALS
  ex_m.CONTROL_SIGNALS = CONTROL_SIGNALS;

  // branch test
  ex_m.BRANCH_DECISION = BRANCH_DECISION;

  // branch delay slot
  ex_m.BRANCH_DELAY_SLOT = BRANCH_DELAY_SLOT;

  // ALU 
  ex_m.ALU_OUTPUT = alu.getResult();

  // RS2
  ex_m.RS2 = RS2;

  // RD
  ex_m.RD = RD;

}

/*
 * Memory
 */

void
MemoryStage::propagate()
{
  // PC
  PC = ex_m.PC;

  // CONTROL_SIGNALS
  CONTROL_SIGNALS = ex_m.CONTROL_SIGNALS;

  // Data to be written to memory
  RegValue DATA = ex_m.RS2;
  // Effective Address
  RegValue EFFECTIVE_ADDRESS = ex_m.ALU_OUTPUT;

  // set the right values if we want to write to memory
  dataMemory.setDataIn(DATA);
  dataMemory.setAddress(EFFECTIVE_ADDRESS);

  // size of the data to be read from memory, so either 1, 2 or 4 bytes
  if (CONTROL_SIGNALS.getDataSize() > 0) {
    dataMemory.setSize(CONTROL_SIGNALS.getDataSize());
  }

  // read from memory
  if (CONTROL_SIGNALS.isReadOpBool()) {
    dataMemory.setReadEnable(true);
    DATA_READ_FROM_MEMORY = dataMemory.getDataOut(CONTROL_SIGNALS.signExtendedRead());
  } else {
    dataMemory.setReadEnable(false);
  }
  dataMemory.setWriteEnable(CONTROL_SIGNALS.isWriteOpBool());

  // The result that we got out of the ALU in the previious clock cycle of this
  // instruction
  ALU_RESULT = ex_m.ALU_OUTPUT;

  // RD
  RD = ex_m.RD;

}

void
MemoryStage::clockPulse()
{
  if(pipelining && HAZARD_DETECTOR.isDataHazard()) {
  }

  // PC
  m_wb.PC = PC;

  // DATA READ FROM MEMORY
  m_wb.DATA_READ_FROM_MEMORY = DATA_READ_FROM_MEMORY;

  // OUTPUT Data Memory
  m_wb.ALU_RESULT = ALU_RESULT;

  // OUTPUT RD
  m_wb.RD = RD;

  // CONTROL SIGNALS
  m_wb.CONTROL_SIGNALS = CONTROL_SIGNALS;

  dataMemory.clockPulse();
}

/*
 * Write back
 */

void
WriteBackStage::propagate()
{
  if (! pipelining || (pipelining && m_wb.PC != 0x0))
    ++nInstrCompleted;
  
  PC = m_wb.PC;
  CONTROL_SIGNALS = m_wb.CONTROL_SIGNALS;

  Mux<RegValue, InputSelectorWBStage> mux;
  mux.setInput(InputSelectorWBStage::InputOne, m_wb.DATA_READ_FROM_MEMORY);
  mux.setInput(InputSelectorWBStage::InputTwo, m_wb.ALU_RESULT);
  mux.setSelector(CONTROL_SIGNALS.isReadOp());

  // Registers INPUT 4(is is RegValue BTW!!!!)
  if (CONTROL_SIGNALS.regWriteInput()) {
    if (CONTROL_SIGNALS.setLinkRegister()) regfile.setWriteData(PC);
    else regfile.setWriteData(mux.getOutput());
      
    regfile.setWriteEnable(true);
    regfile.setRD(m_wb.RD);
  } else {
    regfile.setWriteEnable(false);
  }

}

void
WriteBackStage::clockPulse()
{
  if(pipelining && HAZARD_DETECTOR.isDataHazard()) {
  }

  regfile.clockPulse();
}


// Check if a data hazard is detected
void HazardDetector::detectDataHazard() {
  
  // if the register to be read in the hazard is r0 it is also no data hazard
  // because r0 is always 0 and cannot be overwritten

  if (ex_m.RD != 0 && ex_m.RD != MaxRegs && id_ex.A == ex_m.RD) {
    std::cout << "Data Hazard Detected: ID_EX B = M_WB D" << std::endl;
    dataHazard = true;
    return;
  }

  if (ex_m.RD != 0 && ex_m.RD != MaxRegs && id_ex.B == ex_m.RD) {
    std::cout << "Data Hazard Detected: ID_EX B = M_WB D" << std::endl;
    dataHazard = true;
    return;
  }

  if (m_wb.RD != 0 && m_wb.RD != MaxRegs && id_ex.A == m_wb.RD) {
    std::cout << "Data Hazard Detected: ID_EX A = M_WB D" << std::endl;
    std::cout << "D = " << (int) m_wb.RD << std::endl;
    dataHazard = true;
    return;
  }

  if (m_wb.RD != 0 && m_wb.RD != MaxRegs && id_ex.B == m_wb.RD) {
    std::cout << "Data Hazard Detected: ID_EX B = M_WB D" << std::endl;
    dataHazard = true;
    return;
  }

  dataHazard = false;
}

bool HazardDetector::isDataHazard() const {
  
  if (ex_m.RD != 0 && ex_m.RD != MaxRegs && id_ex.A == ex_m.RD) {
    std::cout << "Data Hazard Detected: ID_EX B = M_WB D" << std::endl;
    return true;
  }

  if (ex_m.RD != 0 && ex_m.RD != MaxRegs && id_ex.B == ex_m.RD) {
    std::cout << "Data Hazard Detected: ID_EX B = M_WB D" << std::endl;
    return true;
  }

  if (m_wb.RD != 0 && m_wb.RD != MaxRegs && id_ex.A == m_wb.RD) {
    std::cout << "Data Hazard Detected: ID_EX A = M_WB D" << std::endl;
    std::cout << "D = " << (int) m_wb.RD << std::endl;
    return true;
  }

  if (m_wb.RD != 0 && m_wb.RD != MaxRegs && id_ex.B == m_wb.RD) {
    std::cout << "Data Hazard Detected: ID_EX B = M_WB D" << std::endl;
    return true;
  }

  return false;

}

