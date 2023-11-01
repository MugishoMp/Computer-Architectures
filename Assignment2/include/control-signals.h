#include "arch.h"
#include "inst-decoder-enums.h"
#include "alu.h"


enum class InputSelector
{
  InputOne,
  InputTwo,
  LAST
};



class ControlSignals
{
public:
    // Constructor to initialize control signals based on instruction word or other parameters
    ControlSignals();

    // Getter methods for various control signals
    InputSelector AInput() const; // AInput
    InputSelector BInput() const;  // BInput
    ALUOp  ALUOp() const; // ALUOp
    
    bool isBranch() const; // isBranch
    bool regWriteInput() const; // regWriteInput

    // Setter methods for various control signals
    void setOpcode(uint16_t opCode) { this->opCode = opCode; };
    void setFunctionCode(InstructionMnemonic functionCode) { this->functionCode = functionCode; };

private:
    uint16_t opCode;
    InstructionMnemonic functionCode;

};