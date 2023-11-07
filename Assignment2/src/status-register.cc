// status_register.cc
#include "status-register.h"

StatusRegister::StatusRegister() {
    flags = 0;
}

void StatusRegister::setFlag(int flag) {
    flags |= flag; // Set the flag bit
}

bool StatusRegister::getFlag(int flag) {
    return (flags & flag) != 0; // Check if the flag bit is set
}

void StatusRegister::clearFlag(int flag) {
    flags &= ~flag; // Clear the flag bit
}

bool StatusRegister::isFlagSet(Flags flag) const {
    return (flags & flag) != 0; // Check if flag is set
}

void StatusRegister::reset() {
    flags = 0; // Reset all flags
}

uint8_t StatusRegister::getFlags() const {
    return flags; // Getter for the flags (if needed)
}