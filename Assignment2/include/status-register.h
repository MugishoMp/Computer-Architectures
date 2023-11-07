// status-register.h
#ifndef STATUS_REGISTER_H
#define STATUS_REGISTER_H

class StatusRegister {
    public:
        enum Flags {
            ZERO = 1 << 0,  // 0b0001
            SIGN = 1 << 1,  // 0b0010
            CARRY = 1 << 2, // 0b0100
            OVERFLOW = 1 << 3, // 0b1000
            AUXCARRY = 1 << 4, // 0b1000
            PARITY = 1 << 5, // 0b1000
            // Add other flags as necessary
        };

    public:

        StatusRegister();
        void setFlag(int flag);
        bool getFlag(int flag);
        void clearFlag(int flag);
        bool isFlagSet(Flags flag) const;
        void reset();
        uint8_t getFlags() const;

    private:
        uint8_t flags = 0; // Use an integer to store flags as bits
};

#endif // STATUS_REGISTER_H
