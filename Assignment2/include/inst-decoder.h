/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    inst-decoder.h - RISC-V instruction decoder.
 *
 * Copyright (C) 2016,2019  Leiden University, The Netherlands.
 */

#ifndef __INST_DECODER_H__
#define __INST_DECODER_H__

#include "reg-file.h"
#include "inst-decoder-enums.h"
#include "inst-decoder-bitmasks.h"

#include <stdexcept>
#include <cstdint>

static const int INSTRUCTION_SIZE = 4;


/* Exception that should be thrown when an illegal instruction
 * is encountered.
 */
class IllegalInstruction : public std::runtime_error
{
  public:
    explicit IllegalInstruction(const std::string &what)
      : std::runtime_error(what)
    { }

    explicit IllegalInstruction(const char *what)
      : std::runtime_error(what)
    { }
};


/* InstructionDecoder component to be used by class Processor */

/*
  * A thing to understand is that the location of these fields is in general always the same, except for some cases. 
  * What does change however is what you will be getting.
  * So which combination of getters you will be using. And this depends on on the instruction type that we are using.
  * 
  * */
class InstructionDecoder
{
  public:
    /**
     * Set the instruction word for decoding.
     * @param instructionWord The 32-bit instruction word.
     */
    void setInstructionWord(const uint32_t instructionWord);

    /**
     * Get the instruction word currently set for decoding.
     * @return The 32-bit instruction word.
     */
    uint32_t getInstructionWord() const;

    // /**
    //  * Set the instruction type.
    //  * @param instructionType The type of the instruction (R, I, S, SH, J, F).
    //  */
    // void setInstructionType(const Type instructionType);

    /**
     * Get the instruction type.
     * @return The type of the instruction.
     */
    InstructionType getInstructionType() const;

    /**
     * Get the opcode of the instruction.
     * @brief gets the bits at the bit positions 31-26
     * in the case of a R, I, S, SH or F type instruction
     * @return The opcode.
     */
    uint16_t getOpcode() const;
    
    /**
     * Get the op2 field of the instruction.
     * @brief gets the bits at the bit positions 9-8
     * in the case of an R type instruction or the bits
     * 7-6 in the case of an SH type instruction
     * @return The op2 field.
     */
    uint32_t getOp2() const;

    /**
     * Get the op3 field of the instruction.
     * @brief gets the bits at the bit positions 3-0
     * in the case of a R type instruction
     * @return The op3 field.
     */
    uint8_t getOp3() const;
    
    /**
     * Get the value of register A.
     * @brief gets the bits at the bit positions 20-16
     * in the case of a R, I, S, SH or F type instruction
     * @return The value of register A.
     */
    RegNumber getA() const;

    /**
     * Get the value of register B.
     * @brief gets the bits at the bit positions 15-11
     * in the case of a R or S type instruction
     * @return The value of register B.
     */
    RegNumber getB() const;

    /**
     * Get the value of register D.
     * @brief gets the bits at the bit positions 25-21
     * in the case of a R, I or SH type instruction
     * @return The value of register D.
     */
    RegNumber getD() const;

    /**
     * Get the immediate I value.
     * @brief gets the bits at the bit positions 15-0
     * in the case of a I or F type instruction or the bits
     * 10-0 in the case of an S type instruction
     * @return The immediate value I.
     */
    int16_t getImmediateI() const;

    /**
     * Get the immediate N value.
     * @brief gets the bits at the bit positions 25-0
     * in the case of a J type instruction 
     * @return The immediate N.
     */
    int32_t getImmediateN() const;

    
    /**
     * Get the immediate value.
     * @brief gets either the immediate I or N value and sign extends this value
     *        so that it fits 32 bits
     * @return The immediate N.
     */
    int32_t getImmediate() const;

    /**
     * Get the reserved field of the instruction.
     * @brief gets the bits at the bit position 10 and 7-4
     * in the case of a R type instruction or the bits
     * 15-8 in the case of an S type instruction
     * @return The reserved field.
     */
    int32_t getReserved() const;

    /**
     * Get the L field of the instruction.
     * @brief gets the bits at the bit positions 5-0
     * in the case of a SH type instruction
     * @return The L field.
     */
    int8_t getL() const;

    /**
     * @brief 
     * 
     * @return uint32_t 
     */
    int32_t getK() const;

    /**
     * @brief 
     * 
     * @return uint8_t 
     */ 
    int8_t getO() const;
    
    /**
     * Get FunctionCode of the instruction.
     * @brief gets the bits at the bit positions
     * in the case of a R type instruction
     * @return The FunctionCode.
     */
    InstructionMnemonic getFunctionCode() const;

    InstructionMnemonic getFunctionCodeRTypeInstruction() const;
    InstructionMnemonic getFunctionCodeITypeInstruction() const;
    InstructionMnemonic getFunctionCodeSTypeInstruction() const;
    InstructionMnemonic getFunctionCodeSHTypeInstruction() const;
    InstructionMnemonic getFunctionCodeJTypeInstruction() const;
    InstructionMnemonic getFunctionCodeFTypeInstruction() const;
    InstructionMnemonic getFunctionCodeDNTypeInstruction() const;
    InstructionMnemonic getFunctionCodeORKTypeInstruction() const;
    InstructionMnemonic getFunctionCodeDROKTypeInstruction() const;
    InstructionMnemonic getFunctionCodeOKTypeInstruction() const;
    InstructionMnemonic getFunctionCodeRESTypeInstruction() const;
    //   InstructionMnemonic getFunctionCodeDABROOTypeInstruction();
    InstructionMnemonic getFunctionCodeRBRTypeInstruction() const;
    InstructionMnemonic getFunctionCodeRAITypeInstruction() const;
    InstructionMnemonic getFunctionCodeDAKTypeInstruction() const;
    InstructionMnemonic getFunctionCodeKABKTypeInstruction() const;
    InstructionMnemonic getFunctionCodeRABROTypeInstruction() const;
    InstructionMnemonic getFunctionCodeOABRTypeInstruction() const;
    InstructionMnemonic getFunctionCodeDABLKTypeInstruction() const;

  private:
    uint32_t instructionWord;
    InstructionType instructionType;
};

std::ostream &operator<<(std::ostream &os, const InstructionDecoder &decoder);

#endif /* __INST_DECODER_H__ */
