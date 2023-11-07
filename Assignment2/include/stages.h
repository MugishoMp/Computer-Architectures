/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    stages.h - Pipeline stages
 *
 * Copyright (C) 2016-2020  Leiden University, The Netherlands.
 */

#ifndef __STAGES_H__
#define __STAGES_H__

#include "alu.h"
#include "mux.h"
#include "inst-decoder.h"
#include "memory-control.h"
#include "control-signals.h"




/* Pipeline registers may be read during propagate and may only be
 * written during clockPulse. Note that you cannot read the incoming
 * pipeline registers in clockPulse (e.g. in clockPulse of EX, you cannot
 * read ID_EX) because that register will already have been overwritten.
 * In case you need to propagate values from one pipeline register to
 * the next, these need to be buffered explicitly within the stage.
 */
struct IF_IDRegisters
{
  MemAddress PC = 0;

  /* TODO: add necessary fields */
  RegValue INSTRUCTION_WORD{};
};

struct ID_EXRegisters
{
  MemAddress PC{};

  /* TODO: add necessary fields */
  RegValue RS1{};
  RegValue RS2{};
  RegNumber RD{};
  RegValue IMMEDIATE{};
  ControlSignals CONTROL_SIGNALS;
};

struct EX_MRegisters
{
  MemAddress PC{};

  /* TODO: add necessary fields */
  ControlSignals CONTROL_SIGNALS;
  // zero? output
  // ALU output
  RegValue ALU_OUTPUT{};
  RegValue RS2{};
  RegNumber RD{};

  RegValue BRANCH_PC{};
  InputSelectorIFStage BRANCH_DECISION{};
};

struct M_WBRegisters
{
  MemAddress PC{};

  /* TODO: add necessary fields */
  ControlSignals CONTROL_SIGNALS;
  RegValue DATA_READ_FROM_MEMORY{};
  RegValue ALU_RESULT{};
  RegNumber RD{};
};


/*
 * Abstract base class for pipeline stage
 */

class Stage
{
  public:
    Stage(bool pipelining)
      : pipelining(pipelining)
    { }

    virtual ~Stage()
    { }

    virtual void propagate() = 0;
    virtual void clockPulse() = 0;

  protected:
    bool pipelining;
};


/*
 * Instruction fetch
 */

class InstructionFetchFailure : public std::exception
{
  public:
    explicit InstructionFetchFailure(const MemAddress addr)
    {
      std::stringstream ss;
      ss << "Instruction fetch failed at address " << std::hex << addr;
      message = ss.str();
    }

    const char* what() const noexcept override
    {
      return message.c_str();
    }

  private:
    std::string message{};
};

class TestEndMarkerEncountered : public std::exception
{
  public:
    explicit TestEndMarkerEncountered(const MemAddress addr)
    {
      std::stringstream ss;
      ss << "Test end marker encountered at address " << std::hex << addr;
      message = ss.str();
    }

    const char* what() const noexcept override
    {
      return message.c_str();
    }

  private:
    std::string message{};
};


class InstructionFetchStage : public Stage
{
  public:
    InstructionFetchStage(bool pipelining,
                          const EX_MRegisters &ex_m,
                          IF_IDRegisters &if_id,
                          InstructionMemory instructionMemory,
                          MemAddress &PC)
      : Stage(pipelining),
      ex_m(ex_m),
      if_id(if_id),
      instructionMemory(instructionMemory),
      PC(PC)
    { }

    void propagate() override;
    void clockPulse() override;

  private:
    const EX_MRegisters &ex_m;
    IF_IDRegisters &if_id;

    InstructionMemory instructionMemory;
    MemAddress &PC;
};

/*
 * Instruction decode
 */

class InstructionDecodeStage : public Stage
{
  public:
    InstructionDecodeStage(bool pipelining,
                           const IF_IDRegisters &if_id,
                           const M_WBRegisters &m_wb,
                           ID_EXRegisters &id_ex,
                           RegisterFile &regfile,
                           InstructionDecoder &decoder,
                           uint64_t &nInstrIssued,
                           uint64_t &nStalls,
                           bool debugMode = false)
      : Stage(pipelining),
      if_id(if_id), id_ex(id_ex), m_wb(m_wb),
      regfile(regfile), decoder(decoder),
      nInstrIssued(nInstrIssued), nStalls(nStalls),
      debugMode(debugMode)
    { }

    void propagate() override;
    void clockPulse() override;

  private:
    const IF_IDRegisters &if_id;
    const M_WBRegisters &m_wb;
    ID_EXRegisters &id_ex;

    RegisterFile &regfile;
    InstructionDecoder &decoder;

    uint64_t &nInstrIssued;
    uint64_t &nStalls;

    bool debugMode;

    MemAddress PC{};
    /* TODO: add other necessary fields/buffers. */
    RegValue SIGN_EXTENDED_IMMEIDATE;
    ControlSignals CONTROL_SIGNALS;
};

/*
 * Execute
 */

class ExecuteStage : public Stage
{
  public:
    ExecuteStage(bool pipelining,
                 const ID_EXRegisters &id_ex,
                 EX_MRegisters &ex_m)
      : Stage(pipelining),
      id_ex(id_ex), ex_m(ex_m)
    { }

    void propagate() override;
    void clockPulse() override;

  private:
    const ID_EXRegisters &id_ex;
    EX_MRegisters &ex_m;

    MemAddress PC{};
    /* TODO: add other necessary fields/buffers and components (ALU anyone?) */
    ALU alu;
    RegValue RS2{};
    RegNumber RD{};
    InputSelectorIFStage BRANCH_DECISION{};
    ControlSignals CONTROL_SIGNALS;

    // bool FLAG;     uninitializedFlag is not initialized here, so it has an 
    //                indeterminate value
    //  bool FLAG{};  initializedFlag is initialized to false due to the {}
    bool FLAG{};
    bool ZERO_FLAG{};
    bool SIGN_FLAG{};
    bool CARRY_FLAG{};
    bool OVERFLOW_FLAG{};
};

/*
 * Memory
 */

class MemoryStage : public Stage
{
  public:
    MemoryStage(bool pipelining,
                const EX_MRegisters &ex_m,
                M_WBRegisters &m_wb,
                DataMemory dataMemory)
      : Stage(pipelining),
      ex_m(ex_m), m_wb(m_wb), dataMemory(dataMemory)
    { }

    void propagate() override;
    void clockPulse() override;

  private:
    const EX_MRegisters &ex_m;
    M_WBRegisters &m_wb;

    DataMemory dataMemory;

    MemAddress PC{};
    /* TODO: add other necessary fields/buffers */
    RegValue DATA_READ_FROM_MEMORY{};
    RegValue ALU_RESULT{};
    RegNumber RD{};
    ControlSignals CONTROL_SIGNALS;
};

/*
 * Write back
 */

class WriteBackStage : public Stage
{
  public:
    WriteBackStage(bool pipelining,
                   const M_WBRegisters &m_wb,
                   RegisterFile &regfile,
                   bool &flag,
                   uint64_t &nInstrCompleted)
      : Stage(pipelining),
      m_wb(m_wb), regfile(regfile), flag(flag),
      nInstrCompleted(nInstrCompleted)
    { }

    void propagate() override;
    void clockPulse() override;

  private:
    const M_WBRegisters &m_wb;

    RegisterFile &regfile;
    bool &flag;

    /* TODO add other necessary fields/buffers and components */

    MemAddress PC{};
    uint64_t &nInstrCompleted;
    ControlSignals CONTROL_SIGNALS;
};

#endif /* __STAGES_H__ */
