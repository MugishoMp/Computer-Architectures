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
    // Constructor to initialize control signals based on instruction word or other parameters
    ControlSignals();

    // Getter methods for various control signals
    InputSelectorEXStage AInput() const; // AInput
    InputSelectorEXStage BInput() const;  // BInput
    ALUOp AluOp() const; // ALUOp
    
    bool isBranch() const; // isBranch
    bool regWriteInput() const; // regWriteInput
    InputSelectorWBStage isReadOp() const;
    bool isReadOpBool() const;
    bool isWriteOpBool() const;
    uint8_t getDataSize() const;
    bool jump(bool FLAG) const;
    bool setLinkRegister() const;

    // Setter methods for various control signals
    void setOpcode(uint16_t newOpCode) { this->opCode = newOpCode; };
    void setFunctionCode(InstructionMnemonic newFunctionCode) { this->functionCode = newFunctionCode; };


private:
    uint16_t opCode;
    InstructionMnemonic functionCode;

};