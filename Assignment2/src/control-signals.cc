#include "control-signals.h"


ControlSignals::ControlSignals()
  : opCode(), functionCode() 
{
}

InputSelector ControlSignals::AInput() const
{
  InputSelector input = InputSelector::InputTwo;
  if (isBranch()) {
    InputSelector input = InputSelector::InputOne;
  }
  return input;
}

InputSelector ControlSignals::BInput() const
{
    
}

ALUOp ControlSignals::ALUOp() const
{
    switch (this->opCode)
    {
    case /* constant-expression */:
        /* code */
        break;
    
    default:
        break;
    }
}
    

bool ControlSignals::regWriteInput() const
{
    return false;
}

bool ControlSignals::isBranch() const
{
    bool result = false;
    // switch case statement
    if
    return result;
}
