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

// Define the operations
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

// Define the flags
enum class ALUFlags {
    Zero,    // Set if the result is zero
    Sign,    // Set if the result is negative
    Carry,   // Set if there is a carry out
    Overflow // Set if there is an overflow
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
    void setOp(ALUOp op) { this->op = op; }

    RegValue getResult();

    // Methods to retrieve the status of the flags
    bool getZeroFlag() const { return zeroFlag; }
    bool getSignFlag() const { return signFlag; }
    bool getCarryFlag() const { return carryFlag; }
    bool getOverflowFlag() const { return overflowFlag; }

  private:
    RegValue A;
    RegValue B;

    ALUOp op;

    // Flags
    bool zeroFlag = false;
    bool signFlag = false;
    bool carryFlag = false;
    bool overflowFlag = false;
    
    // Method to update flags
    void updateFlags(RegValue result);
};

#endif /* __ALU_H__ */
