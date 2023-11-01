/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    alu.h - ALU component.
 *
 * Copyright (C) 2016  Leiden University, The Netherlands.
 */

#ifndef __ALU_H__
#define __ALU_H__

#include "arch.h"
#include "inst-decoder.h"

#include <map>

enum class ALUOp {
    NOP,  // No operation

    // Arithmetic Operations
    ADD,  // Addition
    SUB,  // Subtraction
    MUL,  // Multiplication
    DIV,  // Division
    MOD,  // Modulus

    // Logical Operations
    AND,  // Bitwise AND
    OR,   // Bitwise OR
    XOR,  // Bitwise XOR
    NOT,  // Bitwise NOT

    // Shift Operations
    SHL,  // Shift Left
    SHR,  // Shift Right (Logical)
    SAR,  // Shift Right (Arithmetic)

    // Comparison Operations
    EQ,   // Equal
    NEQ,  // Not Equal
    LT,   // Less Than
    LTE,  // Less Than or Equal
    GT,   // Greater Than
    GTE,  // Greater Than or Equal

    // Other Operations
    INC,  // Increment
    DEC   // Decrement
};


/* The ALU component performs the specified operation on operands A and B
 * when asked to propagate the result. The operation is specified through
 * the ALUOp.
 */
class ALU
{
  public:
    ALU();

    void setA(RegValue A) { this->A = A; }
    void setB(RegValue B) { this->B = B; }

    RegValue getResult();

    void setOp(ALUOp op) { this->op = op; }

  private:
    RegValue A;
    RegValue B;

    ALUOp op;
};

#endif /* __ALU_H__ */
