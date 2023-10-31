/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    stages.cc - Pipeline stages
 *
 * Copyright (C) 2016-2020  Leiden University, The Netherlands.
 */

#include "stages.h"

#include <iostream>

/*
 * Instruction fetch
 */

void
InstructionFetchStage::propagate()
{
  try
    {
      // This is done by sending the PC to the instruction memory setAddress line
      instructionMemory.setAddress(PC);
      // and setting the size to 4 bytes, as OpenRISC instructions are 32 bits in size.
      instructionMemory.setSize(4);
      // at the same time the program counter is incremented by 4
      PC += 4;

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
  /* TODO: write necessary fields in pipeline register */
  // the new sequential pc is ledged into the pc register
  if_id.PC = PC;
  // Also pass the instruction word to the next stage through the pipeline register.
  // Subsequently, the instruction can be loaded by reading the value lines. 
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
  /* TODO: set instruction word on the instruction decoder */
  /* TODO: register fetch and other matters */
  /* TODO: perhaps also determine and write the new PC here? */

  PC = if_id.PC;
  
  // while the instruction stored in the IF/ID pipeline register is being
  // decoded in the control unit, which for simplicitly is not shown in 
  // the block diagram
  decoder.setInstructionWord(if_id.INSTRUCTION_WORD);


  // the registers RS and RT are read from the regsiter file in case we need them
  try {
    regfile.setRS1(decoder.getA());
  } catch (IllegalInstruction &e) {
    // the field RS uses brace initiazliation so it will be zero-initialized
    // but we really want to distinguish that RS1 is not a register
    regfile.setRS1(MaxRegs);
  }
  try {
    regfile.setRS2(decoder.getB());
  } catch (IllegalInstruction &e) {
    // same case here
    regfile.setRS1(MaxRegs);
  }

  


  try {
    regfile.setRD(decoder.getD());
  } catch (IllegalInstruction &e) {}


  // try {
  //   regfile.setWriteData(0);
  // } catch (IllegalInstruction &e) {}

  // try {
  //   regfile.setWriteEnable(0); 
  // } catch (IllegalInstruction &e) {}


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
  /* ignore the "instruction" in the first cycle. */
  if (! pipelining || (pipelining && PC != 0x0))
    ++nInstrIssued;

  /* TODO: write necessary fields in pipeline register */
  id_ex.PC = PC;

  // the registers RS and RT are read from the regsiter file in 
  // case we need them and stored in the ID/EX pipeline register
  id_ex.RS = regfile.getReadData1();
  id_ex.RT = regfile.getReadData2();

  // the 16-bit immediate encoded in the instruction is peculiartively sign extended to
  // 32 bits

  // the destination register RD and the the next sequential register PC are passed to
  // the ID/EX pipeline register because they are needed in later stages 
  id_ex.RD = decoder.getD();
  id_ex.IMMEDIATE = decoder.getImmediateI();
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

  PC = id_ex.PC;
}

void
ExecuteStage::clockPulse()
{
  /* TODO: write necessary fields in pipeline register. This
   * includes the result (output) of the ALU. For memory-operations
   * the ALU computes the effective memory address.
   */

  ex_m.PC = PC;
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
}

void
MemoryStage::clockPulse()
{
  /* TODO: pulse the data memory */

  /* TODO: write necessary fields in pipeline register */

  m_wb.PC = PC;
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
}

void
WriteBackStage::clockPulse()
{
  /* TODO: pulse the register file */
}
