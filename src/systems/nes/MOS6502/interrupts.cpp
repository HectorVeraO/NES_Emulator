//
// Created by junds on 19-09-2021.
//

#include "MOS6502.h"

void MOS6502::interrupt(uint16_t pclAddress, uint16_t pchAddress, uint8_t preservedP) {
    opcycles = 7;
    uint8_t PCL = PC & 0x00FF;
    uint8_t PCH = (PC & 0xFF00) >> 8;
    pushStack(PCH);
    pushStack(PCL);
    pushStack(preservedP);
    PCL = readMemory(pclAddress);
    PCH = readMemory(pchAddress);
    PC = (PCH << 8) | PCL;
}

void MOS6502::interruptNONE() {

}

void MOS6502::interruptNMI() {
    uint8_t preservedP = P;
    preservedP |= ~(1 << 4);
    preservedP |= (1 << 5);
    setFlag(Flag::B, false);
    setFlag(Flag::BreakCommand, false);
    setFlag(Flag::InterruptDisable, true);
    interrupt(0xFFFA, 0xFFFB, preservedP);
}

void MOS6502::interruptIRQ() {
    if (!getFlag(Flag::InterruptDisable)) {
        uint8_t preservedP = P;
        preservedP &= ~(1 << 4);
        preservedP |= (1 << 5);
        setFlag(Flag::B, false);
        setFlag(Flag::InterruptDisable, true);
        interrupt(0xFFFE, 0xFFFF, preservedP);
    }
}

void MOS6502::interruptReset() {
    uint8_t preservedP = P;
    preservedP &= ~(1 << 4);
    preservedP |= (1 << 5);
    interrupt(0xFFFC, 0xFFFD, preservedP);
    setFlag(Flag::B, true);
}
