/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    alu.h - ALU component.
 *
 * Copyright (C) 2016,2018  Leiden University, The Netherlands.
 */

#include "alu.h"

#include "inst-decoder.h"
#include <iostream>

#ifdef _MSC_VER
/* MSVC intrinsics */
#include <intrin.h>
#endif


ALU::ALU()
  : A(), B(), op()
{
}


RegValue
ALU::getResult()
{
    RegValue result = 0;

    switch (op)
    {
        case ALUOp::NOP:
            // No operation
            break;

        // Arithmetic Operations
        case ALUOp::ADD:
            result = A + B;
            break;
        case ALUOp::SUB:
            result = A - B;
            break;
        case ALUOp::MUL:
            result = A * B;
            break;
        case ALUOp::DIV:
            if (B != 0) {
                result = A / B;
            } else {
                throw IllegalInstruction("Division by zero");
            }
            break;
        case ALUOp::MOD:
            if (B != 0) {
                result = A % B;
            } else {
                throw IllegalInstruction("Modulus by zero");
            }
            break;

        // Logical Operations
        case ALUOp::AND:
            result = A & B;
            break;
        case ALUOp::OR:
            result = A | B;
            break;
        case ALUOp::XOR:
            result = A ^ B;
            break;
        case ALUOp::NOT:
            result = ~A;
            break;

        // Shift Operations
        // we ignore the 5th bit of B
        case ALUOp::SHL:


            // Perform the shift operation
            result = A << (int)(B & ~(1 << 5));
            break;
        // Shift Operations
        case ALUOp::SHL_16:
            result = B << 16;
            break;
        // we ignore the 5th bit of B
        case ALUOp::SHR:
            result = 66;
            std::cout << "TESTESTEST" << std::endl;
            break;
        case ALUOp::SAR:
            // Check if the highest-order bit is set
            if (A & (1 << (sizeof(RegValue) * 8 - 1))) {
                // Perform the right shift operation
                result = A >> B;
                
                // Manually set the empty higher-order bits
                for (int i = 0; i < B; ++i) {
                    result |= (1 << (sizeof(RegValue) * 8 - 1 - i));
                }
            } else {
                // If the highest-order bit is not set, then SAR is the same as SHR
                result = A >> B;
            }
            break;

        // Comparison Operations
        case ALUOp::EQ:
            result = (A == B);
            break;
        case ALUOp::NEQ:
            result = (A != B);
            break;
        case ALUOp::LT:
            result = (A < B);
            break;
        case ALUOp::LTE:
            result = (A <= B);
            break;
        case ALUOp::GT:
            result = (A > B);
            break;
        case ALUOp::GTE:
            result = (A >= B);
            break;

        // Other Operations
        case ALUOp::INC:
            result = A + 1;
            break;
        case ALUOp::DEC:
            result = A - 1;
            break;
        case ALUOp::SHIFT_ADD:
            // we do - 2 because we need to take the current pc, not the next one
            result = (A - 4) + (B << 2);
            break;
        case ALUOp::A:
            result = A;
            break;
        case ALUOp::B:
            result = B;
            break;
    }

    // After the result is set, call updateFlags
    updateFlags(result);

    return result;
}

// Define the updateFlags method
void ALU::updateFlags(RegValue result)
{
    // Zero flag
    zeroFlag = (result == 0);

    // Sign flag (assuming RegValue is a signed type)
    signFlag = (result < 0);

    // Carry and Overflow flags would depend on the operation and would need to be
    // updated accordingly, possibly using additional information from the operation
    // (e.g., did an addition result in a carry?).
}
