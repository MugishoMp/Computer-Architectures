/* rv64-emu -- Simple 64-bit RISC-V simulator
 *
 *    memory-control.cc - Memory Controller
 *
 * Copyright (C) 2016-2020  Leiden University, The Netherlands.
 */

#include "memory-control.h"
#include <iostream>

InstructionMemory::InstructionMemory(MemoryBus &bus)
  : bus(bus), size(0), addr(0)
{
}

void
InstructionMemory::setSize(const uint8_t size)
{
  if (size != 2 and size != 4)
    throw IllegalAccess("InstructionMemory::SetSize::Invalid size " + std::to_string(size));

  this->size = size;
}

void
InstructionMemory::setAddress(const MemAddress addr)
{
  this->addr = addr;
}

RegValue
InstructionMemory::getValue() const
{
  switch (size)
    {
      case 2:
        return bus.readHalfWord(addr);

      case 4:
        return bus.readWord(addr);

      default:
        throw IllegalAccess("InstructionMemory::GetValue::Invalid size " + std::to_string(size));
    }
}


DataMemory::DataMemory(MemoryBus &bus)
  : bus{ bus }
{
}

void
DataMemory::setSize(const uint8_t size)
{
  // check if not 2 0r 4 throw illefgal access excecption
  if (size != 1 && size != 2 && size != 4)
    throw IllegalAccess("DataMemory::DataMemory::Invalid size " + std::to_string(size));

  this->size = size;
}

void
DataMemory::setAddress(const MemAddress addr)
{
  this->addr = addr;
}

void
DataMemory::setDataIn(const RegValue value)
{
  this->dataIn = value;
}

void
DataMemory::setReadEnable(bool setting)
{
  readEnable = setting;
}

void
DataMemory::setWriteEnable(bool setting)
{
  writeEnable = setting;
}

RegValue
DataMemory::getDataOut(bool signExtend) const
{
  if (this->size == 1 && this->readEnable){
    if (!signExtend) return this->bus.readByte(this->addr);
    else return (uint32_t)(int32_t)this->bus.readByte(this->addr);

  }  else if (this->size == 2 && this->readEnable){
    if (!signExtend) return this->bus.readHalfWord(this->addr);
    else return (uint32_t)(int32_t)this->bus.readHalfWord(this->addr);

  } else if (this->size == 4 && this->readEnable){
    if (!signExtend) return this->bus.readWord(this->addr);
    else return (uint32_t)(int32_t)this->bus.readWord(this->addr);

  }
  else return 0;
}

void 
DataMemory::clockPulse() const {

  if (this->size == 1 && this->writeEnable)
    this->bus.writeByte(this->addr, static_cast<uint8_t>(this->dataIn));

  else if (this->size == 2 && this->writeEnable)
    this->bus.writeHalfWord(this->addr, static_cast<uint16_t>(this->dataIn));
  
  else if (this->size == 4 && this->writeEnable)
    this->bus.writeWord(this->addr, this->dataIn);
}
