//
// Created by junds on 19-09-2021.
//

#include "MOS6502.h"

void MOS6502::amIMP() {
    addressingMode = AddressingMode::Implicit;
    opaddress = 0;
    opvalue = 0;
}

void MOS6502::amACC() {
    addressingMode = AddressingMode::Accumulator;
    opvalue = A;
    opaddress = 0;
}

void MOS6502::amIMM() {
    addressingMode = AddressingMode::Immediate;
    opaddress = PC;
    opvalue = readMemory(PC++);
}

void MOS6502::amZP() {
    addressingMode = AddressingMode::ZeroPage;
    uint8_t lowByte = readMemory(PC++);
    opaddress = 0x0000 | lowByte;
    opvalue = readMemory(opaddress);
}

void MOS6502::amABS() {
    addressingMode = AddressingMode::Absolute;
    uint16_t lowByte = readMemory(PC++);
    uint16_t highByte = readMemory(PC++) << 8;
    opaddress = highByte | lowByte;
    opvalue = readMemory(opaddress);
}

void MOS6502::amREL() {
    addressingMode = AddressingMode::Relative;
    uint8_t offset = readMemory(PC);
    PC++;
    uint8_t PCL = PC & 0x00FF;
    opaddress = PC + offset;
    opvalue = readMemory(opaddress);
    crossedPageBoundary = offset > (PCL + offset);
}

void MOS6502::amIND() {
    addressingMode = AddressingMode::Indirect;
    uint16_t lowByte = readMemory(PC++);
    uint16_t highByte = readMemory(PC++) << 8;
    lowByte = readMemory(highByte | lowByte);
    highByte = (lowByte + 1) << 8;
    opaddress = highByte | lowByte;
    opvalue = readMemory(opaddress);
}

void MOS6502::amZPX() {
    addressingMode = AddressingMode::ZeroPageX;
    uint8_t lowByte = readMemory(PC++) + X;
    opaddress = 0x0000 | lowByte;
    opvalue = readMemory(opaddress);
}

void MOS6502::amZPY() {
    addressingMode = AddressingMode::ZeroPageY;
    uint8_t lowByte = readMemory(PC++) + Y;
    uint16_t address = 0x0000 | lowByte;
    opvalue = readMemory(address);
}

void MOS6502::amABSX() {
    addressingMode = AddressingMode::AbsoluteX;
    uint16_t lowByte = readMemory(PC++);
    uint16_t highByte = readMemory(PC++) << 8;
    opaddress = (highByte | lowByte) + X;
    opvalue = readMemory(opaddress);
    crossedPageBoundary = X > (lowByte + X);
}

void MOS6502::amABSY() {
    addressingMode = AddressingMode::AbsoluteY;
    uint16_t lowByte = readMemory(PC++);
    uint16_t highByte = readMemory(PC++) << 8;
    opaddress = (highByte | lowByte) + Y;
    opvalue = readMemory(opaddress);
    crossedPageBoundary = Y > (lowByte + Y);
}

void MOS6502::amINDX() {
    addressingMode = AddressingMode::IndexedIndirect;
    uint8_t lowByte = readMemory(PC++) + X;
    uint8_t highByte = lowByte + 1;
    opaddress = (highByte << 8) | lowByte;
    opvalue = readMemory(opaddress);
}

void MOS6502::amINDY() {
    addressingMode = AddressingMode::IndirectIndexed;
    uint8_t lowByte = readMemory(PC++);
    lowByte = readMemory(0x0000 | lowByte);
    lowByte += Y;
    uint8_t carry = lowByte < Y ? 1 : 0;
    uint8_t highByte = readMemory(PC++) + carry;
    opaddress = (highByte << 8) | lowByte;
    opvalue = readMemory(opaddress);
    crossedPageBoundary = carry;
}
