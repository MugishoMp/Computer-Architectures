#include "arch.h"
#include "inst-decoder-enums.h"
#include "alu.h"

enum class InputSelectorIFStage
{
  InputOne,
  InputTwo,
  LAST
};

enum class InputSelectorEXStage
{
  InputOne,
  InputTwo,
  LAST
};

enum class InputSelectorMEMStage
{
  InputOne,
  InputTwo,
  InputTree,
  LAST
};

enum class InputSelectorWBStage
{
  InputOne,
  InputTwo,
  LAST
};



class ControlSignals
{
public:
    /**
     * Default constructor to initialize control signals.
     * Additional setup may be required based on the pipeline stage.
     */
    ControlSignals();

    /**
     * Get the input selector for operand A in the ALU.
     * @return The input selector for operand A.
     */
    InputSelectorEXStage AInput() const;

    /**
     * Get the input selector for operand B in the ALU.
     * @return The input selector for operand B.
     */
    InputSelectorEXStage BInput() const;

    /**
     * Get the ALU operation code.
     * @return The ALU operation code.
     */
    ALUOp AluOp() const;

    /**
     * Check if the instruction is a branch instruction.
     * @return True if the instruction is a branch instruction, false otherwise.
     */
    bool isBranch() const;

    /**
     * Check if the register file should write data.
     * @return True if the register file should write data, false otherwise.
     */
    bool regWriteInput() const;

    /**
     * Get the input selector for the write-back stage.
     * @return The input selector for the write-back stage.
     */
    InputSelectorWBStage isReadOp() const;

    /**
     * Check if the instruction involves a memory read operation.
     * @return True if the instruction involves a memory read, false otherwise.
     */
    bool isReadOpBool() const;

    /**
     * Check if the instruction involves a memory write operation.
     * @return True if the instruction involves a memory write, false otherwise.
     */
    bool isWriteOpBool() const;

    /**
     * Get the size of the data involved in the instruction.
     * @return The size of the data in bytes.
     */
    uint8_t getDataSize() const;

    /**
     * Determine if a jump is required based on a condition.
     * @param FLAG The condition flag.
     * @return True if a jump is required, false otherwise.
     */
    bool jump(bool FLAG) const;

    /**
     * Check if the instruction is setting the link register.
     * @return True if the instruction is setting the link register, false otherwise.
     */
    bool setLinkRegister() const;

    /**
     * Check if sign extension is needed for a memory read operation.
     * @return True if sign extension is needed, false otherwise.
     */
    bool signExtendedRead() const;
    
    // Setter methods for various control signals
    void setOpcode(uint16_t newOpCode) { this->opCode = newOpCode; };
    void setFunctionCode(InstructionMnemonic newFunctionCode) { this->functionCode = newFunctionCode; };


private:
    uint16_t opCode;
    InstructionMnemonic functionCode;

};