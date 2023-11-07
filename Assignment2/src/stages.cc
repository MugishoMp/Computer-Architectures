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
      // INPUT Instruction Memory - PC
      instructionMemory.setAddress(PC);
      instructionMemory.setSize(4);

      // INPUT ADD - PC + 4
      // OUTPUT ADD - NEXT_PC
      MemAddress NEXT_PC = PC + 4;
      RegValue BRANCH_PC = ex_m.ALU_OUTPUT;
      

      // INPUT Mux - ex_m.BRANCH_DECISION
      // INPUT Mux - ex_m.BRANCH_PC
      // INPUT Mux - NEXT_PC
      // OUTPUT Mux - mux.getOutput();
      Mux<RegValue, InputSelectorIFStage> mux;
      mux.setInput(InputSelectorIFStage::InputOne, NEXT_PC );
      mux.setInput(InputSelectorIFStage::InputTwo, BRANCH_PC);
      mux.setSelector(ex_m.BRANCH_DECISION);

      // INPUT PC - mux.getOutput();
      // OUTPUT PC - PC


      /* Enable this once you have implemented instruction fetch. */
      if (instructionMemory.getValue() == TestEndMarker)
        throw TestEndMarkerEncountered(PC);

      PC = mux.getOutput();
      

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
  // OUTPUT Mux - mux.getOutput();
  if_id.PC = PC;
  
  // OUTPUT Instruction Memory
  if_id.INSTRUCTION_WORD = instructionMemory.getValue();
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

  // Registers INPUT 1 - decoder.getA()
  try {
    regfile.setRS1(decoder.getA());
  } catch (IllegalInstruction &e) {
    regfile.setRS1(MaxRegs);
    // the control signal rs1Input needs to return false;
  }

  // Registers INPUT 2 - decoder.getB()
  try {
    regfile.setRS2(decoder.getB());
  } catch (IllegalInstruction &e) {
    regfile.setRS2(MaxRegs);
    // the control signal rs2Input needs to return false;
  }

  // Registers INPUT 3 - RD (the register we want to write data to)
  // handled in the WB stage

  // Registers INPUT 4 - writeData
  // handled in the WB stage

  // Sign Extend INPUT 1
  try {
    SIGN_EXTENDED_IMMEIDATE = decoder.getImmediate();
  } catch (IllegalInstruction &e) {
    SIGN_EXTENDED_IMMEIDATE = 0;
    // the control signal immediateInput needs to return false;
  }
  std::cout << std::hex << PC << std::endl;

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
  // PC
  id_ex.PC = PC;

  id_ex.CONTROL_SIGNALS = CONTROL_SIGNALS;

  // Registers OUTPUT 1
  id_ex.RS1 = regfile.getReadData1();
  // Registers OUTPUT 2
  id_ex.RS2 = regfile.getReadData2();

  // Sign Extend OUTPUT 1
  id_ex.IMMEDIATE = SIGN_EXTENDED_IMMEIDATE;

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
    id_ex.RD = MaxRegs;
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
  /* TODO configure ALU based on control signals and using inputs
   * from pipeline register.
   * Consider using the Mux class.
   */

  // PC
  PC = id_ex.PC;
  CONTROL_SIGNALS = id_ex.CONTROL_SIGNALS;


  // INPUT Mux - id_ex.PC
  // INPUT Mux - id_ex.RS1
  // INPUT Mux (implicit) - CONTROL_SIGNALS.AInput()
  // OUTPUT Mux - mux.getOutput();
  Mux<RegValue, InputSelectorEXStage> mux1;
  mux1.setInput(InputSelectorEXStage::InputOne, PC);
  mux1.setInput(InputSelectorEXStage::InputTwo, id_ex.RS1);
  mux1.setSelector(CONTROL_SIGNALS.AInput());
  

  // INPUT Mux - id_ex.RS2
  // INPUT Mux - id_ex.IMMEDIATE
  // INPUT Mux (implicit) - NEXT_PC
  // OUTPUT Mux - CONTROL_SIGNALS.BInput()
  Mux<RegValue, InputSelectorEXStage> mux2;
  mux2.setInput(InputSelectorEXStage::InputOne, id_ex.RS2);
  mux2.setInput(InputSelectorEXStage::InputTwo, id_ex.IMMEDIATE);
  mux2.setSelector(CONTROL_SIGNALS.BInput());

  // INPUT ALU - mux1.getOutput()
  // INPUT ALU - mux2.getOutput()
  // INPUT ALU (implicit) - operation
  // OUTPUT ALU
  alu.setA(mux1.getOutput());
  alu.setB(mux2.getOutput());
  alu.setOp(CONTROL_SIGNALS.AluOp());

  FLAG = alu.getFlag();
  ZERO_FLAG = alu.getZeroFlag();
  SIGN_FLAG = alu.getSignFlag();
  CARRY_FLAG = alu.getCarryFlag();
  OVERFLOW_FLAG = alu.getOverflowFlag();

  // INPUT FLAG TEST - id_ex.RS1 (if true switch to branch address)
  // INPUT FLAG TEST - id_ex.RS2 (if false take next address)
  // OUTPUT FLAG TEST BRANCH_DECISION
  if (CONTROL_SIGNALS.jump(FLAG)){
    BRANCH_DECISION = InputSelectorIFStage::InputTwo;
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
  /* TODO: write necessary fields in pipeline register. This
   * includes the result (output) of the ALU. For memory-operations
   * the ALU computes the effective memory address.
   */

  // PC
  ex_m.PC = PC;

  // CONTROL_SIGNALS
  ex_m.CONTROL_SIGNALS = CONTROL_SIGNALS;

  // Equality test
  ex_m.BRANCH_DECISION = BRANCH_DECISION;

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
  /* TODO: configure data memory based on control signals and using
   * inputs from pipeline register.
   */

  // PC
  PC = ex_m.PC;

  // CONTROL SIGNALS
  CONTROL_SIGNALS = ex_m.CONTROL_SIGNALS;

  
  RegValue DATA = ex_m.RS2;
  RegValue EFFECTIVE_ADDRESS = ex_m.ALU_OUTPUT;
  dataMemory.setDataIn(DATA);
  dataMemory.setAddress(EFFECTIVE_ADDRESS);

  // size of the data, so either 1, 2 or 4 bytes
  if (ex_m.CONTROL_SIGNALS.getDataSize() > 0) // ???????
    dataMemory.setSize(ex_m.CONTROL_SIGNALS.getDataSize());


  //read
  dataMemory.setReadEnable(ex_m.CONTROL_SIGNALS.isReadOpBool());
  DATA_READ_FROM_MEMORY = dataMemory.getDataOut(false);

  //write
  dataMemory.setWriteEnable(ex_m.CONTROL_SIGNALS.isWriteOpBool());
  dataMemory.clockPulse();

  ALU_RESULT = ex_m.ALU_OUTPUT;

  // RD
  RD = ex_m.RD;
}

void
MemoryStage::clockPulse()
{
  /* TODO: pulse the data memory */

  /* TODO: write necessary fields in pipeline register */


  m_wb.PC = PC;

  m_wb.DATA_READ_FROM_MEMORY = DATA_READ_FROM_MEMORY;

  // OUTPUT Data Memory
  m_wb.ALU_RESULT = ALU_RESULT;

  // OUTPUT RD
  m_wb.RD = RD;

  // CONTROL SIGNALS
  m_wb.CONTROL_SIGNALS = CONTROL_SIGNALS;
}

/*
 * Write back
 */

void
WriteBackStage::propagate()
{
  if (! pipelining || (pipelining && m_wb.PC != 0x0))
    ++nInstrCompleted;

  /* TODO: configure write lines of register file based on control
   * signals
   */
  
  PC = m_wb.PC;
  CONTROL_SIGNALS = m_wb.CONTROL_SIGNALS;

  // INPUT (implicit) Mux - CONTROL_SIGNALS.X()
  // INPUT 1 Mux - m_wb.AAAAAA
  // INPUT 2 Mux - m_wb.BBBBBB
  // OUTPUT Mux - mux.getOutput();
  Mux<RegValue, InputSelectorWBStage> mux;
  mux.setInput(InputSelectorWBStage::InputOne, m_wb.DATA_READ_FROM_MEMORY);
  mux.setInput(InputSelectorWBStage::InputTwo, m_wb.ALU_RESULT);
  // TODO control signal
  mux.setSelector(CONTROL_SIGNALS.isReadOp());

  // Registers INPUT 4(is is RegValue BTW!!!!)
  if (CONTROL_SIGNALS.regWriteInput()) {
    if (CONTROL_SIGNALS.setLinkRegister()) {
      regfile.setWriteData(PC);
    }
    else
      regfile.setWriteData(mux.getOutput());
      
    // TODO control signal
    regfile.setWriteEnable(true);
    regfile.setRD(m_wb.RD);
  } else {
    regfile.setWriteEnable(false);
  }
}

void
WriteBackStage::clockPulse()
{
  /* TODO: pulse the register file */
  regfile.clockPulse();
}
