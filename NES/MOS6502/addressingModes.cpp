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
    uint8_t offset = readMemory(PC++);
    opaddress = PC + offset;
    opvalue = opaddress;
    crossedPageBoundary = (opaddress & 0xFF00) != (PC & 0xFF00);
}

void MOS6502::amIND() {
    addressingMode = AddressingMode::Indirect;
    uint8_t operandLowByte = readMemory(PC++);
    uint8_t operandHighByte = readMemory(PC++);
    uint16_t operand = (operandHighByte << 8) | operandLowByte;
    uint8_t effectiveAddressLowByte = readMemory(operand);
    uint8_t effectiveAddressHighByte = readMemory(operand + 1);
    opaddress = (effectiveAddressHighByte << 8) | effectiveAddressLowByte;
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
    opaddress = 0x0000 | lowByte;
    opvalue = readMemory(opaddress);
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
    uint8_t operand = readMemory(PC++);
    uint8_t sum = operand + X;
    uint8_t effectiveAddressLowByte = readMemory(0x0000 | sum);
    uint8_t effectiveAddressHighByte = readMemory((sum + 1) & 0x00FF);
    opaddress = (effectiveAddressHighByte << 8) | effectiveAddressLowByte;
    opvalue = readMemory(opaddress);
}

// This one was tricky but the documentation from MOS is awesome, http://archive.6502.org/datasheets/mos_6501-6505_mpu_preliminary_aug_1975.pdf
void MOS6502::amINDY() {
    addressingMode = AddressingMode::IndirectIndexed;
    uint8_t operand = readMemory(PC++);
    uint16_t sum = readMemory(0x0000 | operand) + Y;
    bool hasCarry = sum > 0xFF;
    uint8_t effectiveAddressLowByte = sum & 0x00FF;
    uint8_t effectiveAddressHighByte = readMemory(0x0000 | (operand + 1)) + hasCarry;
    opaddress = (effectiveAddressHighByte << 8) | effectiveAddressLowByte;
    opvalue = readMemory(opaddress);
    crossedPageBoundary = hasCarry;
}
