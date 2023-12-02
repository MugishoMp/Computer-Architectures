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
    SHL_16,  // Shift Left
    SHR,  // Shift Right (Logical)
    SAR,  // Shift Right (Arithmetic)

    // Comparison Operations
    EQ,   // Equal
    NEQ,  // Not Equal
    LT,   // Less Than
    LTE,  // Less Than or Equal
    GT,   // Greater Than
    GTE,  // Greater Than or Equal
    
    LTS,   // Less Than
    LTES,  // Less Than or Equal
    GTS,   // Greater Than
    GTES,  // Greater Than or Equal

    // Other Operations
    INC,  // Increment
    DEC,  // Decrement

    SHIFT_ADD, // shift to left with two and add
    A, // return only A
    B, // return only B

    NONE // do nothing
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
    
    // Method to retrieve the result of the ALU operation
    RegValue getResult();

    // Methods to retrieve the status of the flags
    bool getFlag() const { return flag; }              // General purpose flag
    bool getZeroFlag() const { return zeroFlag; }      // Flag indicating zero result
    bool getSignFlag() const { return signFlag; }      // Flag indicating negative result
    bool getCarryFlag() const { return carryFlag; }    // Flag indicating a carry in arithmetic operations
    bool getOverflowFlag() const { return overflowFlag; }  // Flag indicating overflow in arithmetic operations


  private:
    // Input operands for the ALU operation
    RegValue A;
    RegValue B;

    // Operation to be performed by the ALU
    ALUOp op;

    // Flags to represent different conditions after ALU operation
    bool flag = false;          // General purpose flag
    bool zeroFlag = false;      // Flag indicating zero result
    bool signFlag = false;      // Flag indicating negative result
    bool carryFlag = false;     // Flag indicating a carry in arithmetic operations
    bool overflowFlag = false;  // Flag indicating overflow in arithmetic operations

    // Method to update flags based on the result of the ALU operation
    void updateFlags(RegValue result);
};

#endif /* __ALU_H__ */
