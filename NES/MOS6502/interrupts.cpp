//
// Created by junds on 19-09-2021.
//

#include "MOS6502.h"

void MOS6502::interrupt(uint16_t pclAddress, uint16_t pchAddress) {
    opcycles = 7;
    uint8_t PCL = PC & 0x00FF;
    uint8_t PCH = (PC & 0xFF00) >> 8;
    pushStack(PCH);
    pushStack(PCL);
    pushStack(P);
    PCL = readMemory(pclAddress);
    PCH = readMemory(pchAddress);
    PC = (PCH << 8) | PCL;
}

void MOS6502::interruptNONE() {

}

void MOS6502::interruptNMI() {
    setFlag(Flag::B, false);
    interrupt(0xFFFA, 0xFFFB);
}

void MOS6502::interruptIRQ() {
    if (!getFlag(Flag::InterruptDisable)) {
        setFlag(Flag::B, false);
        interrupt(0xFFFE, 0xFFFF);
    }
}

void MOS6502::interruptReset() {
    uint16_t oldPC = PC;
    interrupt(0xFFFC, 0xFFFD);
    PC = oldPC;
    setFlag(Flag::InterruptDisable, true);
}
