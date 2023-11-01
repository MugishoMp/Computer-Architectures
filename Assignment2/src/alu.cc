/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    alu.h - ALU component.
 *
 * Copyright (C) 2016,2018  Leiden University, The Netherlands.
 */

#include "alu.h"

#include "inst-decoder.h"

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
        case ALUOp::SHL:
            result = A << B;
            break;
        case ALUOp::SHR:
            result = A >> B;
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

        default:
            throw IllegalInstruction("Unimplemented or unknown ALU operation");
    }

    return result;
}