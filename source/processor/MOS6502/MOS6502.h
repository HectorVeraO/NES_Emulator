#pragma once

#include <cstdint>
#include "Bus.h"

class Bus;

class MOS6502 {
public:
    MOS6502();

    ~MOS6502();

    void execute();

    void connectBus(Bus *newBus);

private:
    Bus *bus;

    uint16_t PC;  // Program counter
    uint8_t S;    // Stack pointer
    uint8_t P;    // Status register
    uint8_t A;    // Accumulator
    uint8_t X;    // Index X
    uint8_t Y;    // Index Y

    uint8_t opcode{};
    uint8_t opvalue{};      // Value obtained through the addressing mode
    uint16_t opaddress{};   // Address obtained though the addressing mode

    enum AddressingMode {
        Implicit = 0,
        Accumulator = 1,
        Immediate = 2,
        ZeroPage = 3,
        ZeroPageX = 4,
        ZeroPageY = 5,
        Relative = 6,
        Absolute = 7,
        AbsoluteX = 8,
        AbsoluteY = 9,
        Indirect = 10,
        IndexedIndirect = 11,
        IndirectIndexed = 12
    };

    enum Flag {
        Carry = 0,
        Zero = 1,
        InterruptDisable = 2,
        DecimalMode = 3,
        BreakCommand = 4,
        B = 5,
        Overflow = 6,           // Indicates if there's a two's complement overflow
        Negative = 7
    };

    AddressingMode addressingMode;

    uint8_t readMemory(uint16_t address) const;

    void writeMemory(uint16_t address, uint8_t value) const;

    uint8_t pullStack(uint8_t lowByte);

    void pushStack(uint8_t lowByte, uint8_t value);

    uint8_t getFlag(uint8_t offset);

    void setFlag(uint8_t offset, bool turnOn);

    // Addressing modes
    void amIMP();

    void amACC();

    void amIMM();

    void amZP();

    void amABS();

    void amREL();

    void amIND();

    void amZPX();

    void amZPY();

    void amABSX();

    void amABSY();

    void amINDX();

    void amINDY();

    // Legal instructions

    uint8_t opADC();

    uint8_t opAND();

    uint8_t opASL();

    uint8_t opBCC();

    uint8_t opBCS();

    uint8_t opBEQ();

    uint8_t opBIT();

    uint8_t opBMI();

    uint8_t opBNE();

    uint8_t opBPL();

    uint8_t opBRK();

    uint8_t opBVC();

    uint8_t opBVS();

    uint8_t opCLC();

    uint8_t opCLD();

    uint8_t opCLI();

    uint8_t opCLV();

    uint8_t opCMP();

    uint8_t opCPX();

    uint8_t opCPY();

    uint8_t opDEC();

    uint8_t opDEX();

    uint8_t opDEY();

    uint8_t opEOR();

    uint8_t opINC();

    uint8_t opINX();

    uint8_t opINY();

    uint8_t opJMP();

    uint8_t opJSR();

    uint8_t opLDA();

    uint8_t opLDX();

    uint8_t opLDY();

    uint8_t opLSR();

    uint8_t opNOP();

    uint8_t opORA();

    uint8_t opPHA();

    uint8_t opPHP();

    uint8_t opPLA();

    uint8_t opPLP();

    uint8_t opROL();

    uint8_t opROR();

    uint8_t opRTI();

    uint8_t opRTS();

    uint8_t opSBC();

    uint8_t opSEC();

    uint8_t opSED();

    uint8_t opSEI();

    uint8_t opSTA();

    uint8_t opSTX();

    uint8_t opSTY();

    uint8_t opTAX();

    uint8_t opTAY();

    uint8_t opTSX();

    uint8_t opTXA();

    uint8_t opTXS();

    uint8_t opTYA();

    static void opUnimplementedInstruction();
};