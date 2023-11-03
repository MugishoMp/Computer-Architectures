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
      // INPUT ADD - PC + 4
      // OUTPUT ADD - NEXT_PC
      MemAddress NEXT_PC = PC + 4;

      // INPUT Mux - ex_m.BRANCH_DECISION
      // INPUT Mux - ex_m.BRANCH_PC
      // INPUT Mux - NEXT_PC
      // OUTPUT Mux - mux.getOutput();
      Mux<RegValue, InputSelectorIFStage> mux;
      mux.setInput(InputSelectorIFStage::InputOne, ex_m.BRANCH_PC);
      mux.setInput(InputSelectorIFStage::InputTwo, NEXT_PC);
      mux.setSelector(ex_m.BRANCH_DECISION);

      // INPUT Instruction Memory - PC
      instructionMemory.setAddress(PC);
      instructionMemory.setSize(4);

      // INPUT PC - mux.getOutput();
      // OUTPUT PC - PC
      PC = mux.getOutput();
      

#if 0
      /* Enable this once you have implemented instruction fetch. */
      if (instructionWord == TestEndMarker)
        throw TestEndMarkerEncountered(PC);
#endif
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
  // decode instruction
  decoder.setInstructionWord(if_id.INSTRUCTION_WORD);
  // set control signals
  controlSignals.setOpcode(decoder.getOpcode());
  controlSignals.setFunctionCode(decoder.getFunctionCode());

  // PC
  PC = if_id.PC;
  

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
    regfile.setRS1(MaxRegs);
    // the control signal rs2Input needs to return false;
  }

  // Registers INPUT 3 - m_wb.RD
  regfile.setRD(m_wb.RD);

  // Registers INPUT 4
  // handled in the WB stage

  // Sign Extend INPUT 1
  try {
    signExtendedImmediate = decoder.getImmediate();
  } catch (IllegalInstruction &e) {
    signExtendedImmediate = 0;
    // the control signal immediateInput needs to return false;
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
  // PC
  id_ex.PC = PC;
  
  // Registers OUTPUT 1
  id_ex.RS1 = regfile.getReadData1();
  // Registers OUTPUT 2
  id_ex.RS2 = regfile.getReadData2();

  // Sign Extend OUTPUT 1
  id_ex.IMMEDIATE = signExtendedImmediate;

  // Register RD
  id_ex.RD = decoder.getD();

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

  // INPUT FLAG TEST - id_ex.RS1 (if true switch to branch address)
  // INPUT FLAG TEST - id_ex.RS2 (if false take next address)
  // OUTPUT FLAG TEST EQUALITY_TEST
  if (id_ex.RS1 == id_ex.RS2)
    EQUALITY_TEST = InputSelectorIFStage::InputOne;
  else
    EQUALITY_TEST = InputSelectorIFStage::InputTwo;


  // INPUT Mux - id_ex.PC
  // INPUT Mux - id_ex.RS1
  // INPUT Mux (implicit) - CONTROL_SIGNALS.AInput()
  // OUTPUT Mux - mux.getOutput();
  Mux<RegValue, InputSelectorEXStage> mux1;
  mux1.setInput(InputSelectorEXStage::InputOne, id_ex.PC);
  mux1.setInput(InputSelectorEXStage::InputTwo, id_ex.RS1);
  mux1.setSelector(id_ex.CONTROL_SIGNALS.AInput());
  

  // INPUT Mux - id_ex.RS2
  // INPUT Mux - id_ex.IMMEDIATE
  // INPUT Mux (implicit) - NEXT_PC
  // OUTPUT Mux - CONTROL_SIGNALS.BInput()
  Mux<RegValue, InputSelectorEXStage> mux2;
  mux2.setInput(InputSelectorEXStage::InputOne, id_ex.RS2);
  mux2.setInput(InputSelectorEXStage::InputTwo, id_ex.IMMEDIATE);
  mux2.setSelector(id_ex.CONTROL_SIGNALS.BInput());

  // INPUT ALU - mux1.getOutput()
  // INPUT ALU - mux2.getOutput()
  // INPUT ALU (implicit) - operation
  // OUTPUT ALU
  alu.setA(mux1.getOutput());
  alu.setB(mux2.getOutput());
  alu.setOp(id_ex.CONTROL_SIGNALS.ALUOp());

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

  // Equality test
  ex_m.BRANCH_DECISION = EQUALITY_TEST;

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

  PC = ex_m.PC;

  RegValue EFFECTIVE_ADDRESS = ex_m.ALU_OUTPUT;
  
  RegValue DATA = ex_m.RS2;

  dataMemory.setDataIn(DATA);
  dataMemory.setAddress(EFFECTIVE_ADDRESS);

  // size of the data, so either 1, 2 or 4 bytes
  dataMemory.setSize(ex_m.CONTROL_SIGNALS.getDataSize());

  //read
  dataMemory.setReadEnable(ex_m.CONTROL_SIGNALS.isReadOp());
  DATA_READ_FROM_MEMORY = dataMemory.getDataOut(false);

  //write
  dataMemory.setWriteEnable(ex_m.CONTROL_SIGNALS.isWriteOp());
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
  
  // INPUT (implicit) Mux - controlSignals.X()
  // INPUT 1 Mux - m_wb.AAAAAA
  // INPUT 2 Mux - m_wb.BBBBBB
  // OUTPUT Mux - mux.getOutput();
  Mux<RegValue, InputSelectorWBStage> mux;
  mux.setInput(InputSelectorWBStage::InputOne, m_wb.DATA_READ_FROM_MEMORY);
  mux.setInput(InputSelectorWBStage::InputTwo, m_wb.ALU_RESULT);
  // TODO control signal
  mux.setSelector(m_wb.CONTROL_SIGNALS.isReadOp());

  // Registers INPUT 4(is is RegValue BTW!!!!)
  regfile.setWriteData(mux.getOutput());
  // TODO control signal
  regfile.setWriteEnable(m_wb.CONTROL_SIGNALS.isWriteOp());

}

void
WriteBackStage::clockPulse()
{
  /* TODO: pulse the register file */
  regfile.clockPulse();
}
