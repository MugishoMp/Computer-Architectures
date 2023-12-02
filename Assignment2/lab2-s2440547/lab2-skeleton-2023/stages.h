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
  RegValue INSTRUCTION_WORD{0x15000000};
};

struct ID_EXRegisters
{
  MemAddress PC{};

  /* TODO: add necessary fields */
  // Register values for the first source operand
  RegValue RS1{};

  // Register values for the second source operand
  RegValue RS2{};

  // Destination register where the result will be stored
  RegNumber RD{0};

  // Operand A for the ALU operation
  RegNumber A{0};

  // Operand B for the ALU operation
  RegNumber B{0};

  // Sign-extended immediate value from the instruction
  RegValue IMMEDIATE{};

  // Control signals guiding the execution of the instruction
  ControlSignals CONTROL_SIGNALS;
};

struct EX_MRegisters
{
  MemAddress PC{};

  /* TODO: add necessary fields */
  // Control signals guiding the execution of the instruction
  ControlSignals CONTROL_SIGNALS;

  // Output of the ALU operation
  RegValue ALU_OUTPUT{};

  // Value of the second source operand
  RegValue RS2{};

  // Destination register where the result will be stored
  RegNumber RD{0};

  // Program Counter (PC) value for a branch instruction
  RegValue BRANCH_PC{};

  // Decision on whether to take the branch or not
  InputSelectorIFStage BRANCH_DECISION{InputSelectorIFStage::InputOne};

  // Flag indicating the presence of a branch delay slot
  bool BRANCH_DELAY_SLOT{};
};

struct M_WBRegisters
{
  MemAddress PC{};

  /* TODO: add necessary fields */
  ControlSignals CONTROL_SIGNALS;
  RegValue DATA_READ_FROM_MEMORY{};
  RegValue ALU_RESULT{};
  RegNumber RD{0};
};


class HazardDetector {
  public:
    HazardDetector(const IF_IDRegisters& if_id, const ID_EXRegisters& id_ex,
                  const EX_MRegisters& ex_m, const M_WBRegisters& m_wb) 
                  : dataHazard(false),
                    if_id(if_id),
                    id_ex(id_ex),
                    ex_m(ex_m),
                    m_wb(m_wb)
    { }
  void detectDataHazard();
  bool isDataHazard() const;

  private:
    bool dataHazard;
    const IF_IDRegisters &if_id;
    const ID_EXRegisters &id_ex;
    const EX_MRegisters &ex_m;
    const M_WBRegisters &m_wb;
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
                          MemAddress &PC,
                          const HazardDetector &HAZARD_DETECTOR)
      : Stage(pipelining),
      ex_m(ex_m),
      if_id(if_id),
      instructionMemory(instructionMemory),
      PC(PC),
      instr(0),
      HAZARD_DETECTOR(HAZARD_DETECTOR)
    { }

    void propagate() override;
    void clockPulse() override;

  private:
    const EX_MRegisters &ex_m;
    IF_IDRegisters &if_id;

    InstructionMemory instructionMemory;
    MemAddress &PC;

    /* TODO: add other necessary fields/buffers. */
    // Variable to store the current instruction
    RegValue instr;

    // Reference to the Hazard Detector, which identifies hazards in the pipeline
    const HazardDetector &HAZARD_DETECTOR;
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
                           const HazardDetector &HAZARD_DETECTOR,
                           bool debugMode = false)
      : Stage(pipelining),
      if_id(if_id), m_wb(m_wb), id_ex(id_ex),
      regfile(regfile), decoder(decoder),
      nInstrIssued(nInstrIssued), nStalls(nStalls),
      debugMode(debugMode),
      SIGN_EXTENDED_IMMEDIATE(0), // Assuming default initialization to 0
      CONTROL_SIGNALS(),
      HAZARD_DETECTOR(HAZARD_DETECTOR),
      A(0),
      B(0)
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
    // Variable to store the sign-extended immediate value
    RegValue SIGN_EXTENDED_IMMEDIATE;

    // Control signals for the pipeline stage
    ControlSignals CONTROL_SIGNALS;

    // Reference to the Hazard Detector, which identifies hazards in the pipeline
    const HazardDetector &HAZARD_DETECTOR;

    // Register A
    RegNumber A{};

    // Register B
    RegNumber B{};
};

/*
 * Execute
 */

class ExecuteStage : public Stage
{
  public:
    ExecuteStage(bool pipelining,
                 const ID_EXRegisters &id_ex,
                 EX_MRegisters &ex_m,
                 const HazardDetector &HAZARD_DETECTOR)
      : Stage(pipelining),
      id_ex(id_ex), ex_m(ex_m),
      alu(), // Default construction of ALU
      CONTROL_SIGNALS(),
      HAZARD_DETECTOR(HAZARD_DETECTOR)
    { }

    void propagate() override;
    void clockPulse() override;

  private:
    const ID_EXRegisters &id_ex;
    EX_MRegisters &ex_m;

    MemAddress PC{};
    /* TODO: add other necessary fields/buffers and components (ALU anyone?) */
    ALU alu;

    // Register RS2 for the pipeline stage
    RegValue RS2{};

    // Register RD to store the result of ALU operations
    RegNumber RD{0};

    // Branch decision based on the control signals and flags
    InputSelectorIFStage BRANCH_DECISION{};

    // Control signals for the pipeline stage
    ControlSignals CONTROL_SIGNALS;

    // Reference to the Hazard Detector, which identifies hazards in the pipeline
    const HazardDetector &HAZARD_DETECTOR;

    // Flag indicating the result of ALU operations
    bool FLAG{};

    // Flag indicating zero result of ALU operations
    bool ZERO_FLAG{};

    // Flag indicating the sign of the result of ALU operations
    bool SIGN_FLAG{};

    // Flag indicating carry-out in ALU operations
    bool CARRY_FLAG{};

    // Flag indicating overflow in ALU operations
    bool OVERFLOW_FLAG{};

    // Flag indicating whether the pipeline stage is in a branch delay slot
    bool BRANCH_DELAY_SLOT{};
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
                DataMemory dataMemory,
                const HazardDetector &HAZARD_DETECTOR)
      : Stage(pipelining),
      ex_m(ex_m), m_wb(m_wb), dataMemory(dataMemory),
      CONTROL_SIGNALS(),
      HAZARD_DETECTOR(HAZARD_DETECTOR)
    { }

    void propagate() override;
    void clockPulse() override;

  private:
    const EX_MRegisters &ex_m;
    M_WBRegisters &m_wb;

    DataMemory dataMemory;

    MemAddress PC{};
    /* TODO: add other necessary fields/buffers */
    // Control signals for the pipeline stage
    ControlSignals CONTROL_SIGNALS;

    // Register RD to store the result of ALU operations
    RegNumber RD{0};

    // Result of ALU operations
    RegValue ALU_RESULT{};

    // Data read from memory
    RegValue DATA_READ_FROM_MEMORY{};

    // Reference to the Hazard Detector, which identifies hazards in the pipeline
    const HazardDetector &HAZARD_DETECTOR;
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
                   uint64_t &nInstrCompleted,
                   HazardDetector &HAZARD_DETECTOR)
      : Stage(pipelining),
      m_wb(m_wb), regfile(regfile), flag(flag),
      nInstrCompleted(nInstrCompleted),
      CONTROL_SIGNALS(),
      HAZARD_DETECTOR(HAZARD_DETECTOR)
    { }

    void propagate() override;
    void clockPulse() override;

  private:
    const M_WBRegisters &m_wb;
    RegisterFile &regfile;

    /* TODO add other necessary fields/buffers and components */
    // Program Counter (PC) to keep track of the current instruction address
    MemAddress PC{};

    // Reference to a boolean flag
    bool &flag;

    // Reference to a counter tracking the number of completed instructions
    uint64_t &nInstrCompleted;

    // Control signals for the pipeline stage
    ControlSignals CONTROL_SIGNALS;

    // Hazard Detector responsible for identifying hazards in the pipeline
    HazardDetector &HAZARD_DETECTOR;
};


#endif /* __STAGES_H__ */
