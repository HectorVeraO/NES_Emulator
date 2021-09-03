#include <iostream>
#include "MOS6502.h"

MOS6502::MOS6502() {
    bus = nullptr;
    addressingMode = Implicit;
    PC = 0;
    S = 0;
    P = 0;
    A = 0;
    X = 0;
    Y = 0;
    opcode = 0;
}

MOS6502::~MOS6502() = default;

void MOS6502::connectBus(Bus *newBus) {
    this->bus = newBus;
}

uint8_t MOS6502::readMemory(uint16_t address) const {
    return bus->readRAM(address);
}

void MOS6502::writeMemory(uint16_t address, uint8_t value) const {
    bus->writeRAM(address, value);
}

uint8_t MOS6502::pullStack() {
    uint16_t address = 0x0100 | S++;
    uint8_t value = readMemory(address);
    return value;
}

void MOS6502::pushStack(uint8_t value) {
    uint16_t address = 0x0100 | S--;
    writeMemory(address, value);
}

uint8_t MOS6502::getFlag(uint8_t offset) {
    return P & (1 << offset);
}

// TODO: I feel like there is a way to do this without the ternary.
void MOS6502::setFlag(uint8_t offset, bool turnOn) {
    P |= turnOn ? 1 << offset : ~(1 << offset);
}

void MOS6502::execute() {
    opcode = readMemory(PC);
    PC++;

    switch (opcode) {
        case 0x00: {
            amIMP();
            opBRK();
            break;
        }
        case 0x04: {
            amZP();
            opNOP();
            break;
        }
        case 0x08: {
            amIMP();
            opPHP();
            break;
        }
        case 0x0C: {
            amABS();
            opNOP();
            break;
        }
        case 0x10: {
            amREL();
            opBPL();
            break;
        }
        case 0x14: {
            amZPX();
            opNOP();
            break;
        }
        case 0x18: {
            amIMP();
            opCLC();
            break;
        }
        case 0x1C: {
            amABSX();
            opNOP();
            break;
        }

        default: opUnimplementedInstruction();
    }
}

void MOS6502::opUnimplementedInstruction() {
    std::cout << "Operation not implemented\n";
}

void MOS6502::amIMP() {
    addressingMode = Implicit;
    opaddress = 0;
    opvalue = 0;
}

void MOS6502::amACC() {
    addressingMode = Accumulator;
    opvalue = A;
    opaddress = 0;
}

void MOS6502::amIMM() {
    addressingMode = Immediate;
    opaddress = PC;
    opvalue = readMemory(PC++);
}

void MOS6502::amZP() {
    addressingMode = ZeroPage;
    uint8_t lowByte = readMemory(PC++);
    opaddress = 0x0000 | lowByte;
    opvalue = readMemory(opaddress);
}

void MOS6502::amABS() {
    addressingMode = Absolute;
    uint16_t lowByte = readMemory(PC++);
    uint16_t highByte = readMemory(PC++) << 8;
    opaddress = highByte | lowByte;
    opvalue = readMemory(opaddress);
}

void MOS6502::amREL() {
    addressingMode = Relative;
    uint8_t offset = readMemory(PC);
    PC += 2;
    opaddress = PC + offset;
    opvalue = readMemory(opaddress);
}

void MOS6502::amIND() {
    addressingMode = Indirect;
    uint16_t lowByte = readMemory(PC++);
    uint16_t highByte = readMemory(PC++) << 8;
    lowByte = readMemory(highByte | lowByte);
    highByte = (lowByte + 1) << 8;
    opaddress = highByte | lowByte;
    opvalue = readMemory(opaddress);
}

void MOS6502::amZPX() {
    addressingMode = ZeroPageX;
    uint8_t lowByte = readMemory(PC++) + X;
    opaddress = 0x0000 | lowByte;
    opvalue = readMemory(opaddress);
}

void MOS6502::amZPY() {
    addressingMode = ZeroPageY;
    uint8_t lowByte = readMemory(PC++) + Y;
    uint16_t address = 0x0000 | lowByte;
    opvalue = readMemory(address);
}

void MOS6502::amABSX() {
    addressingMode = AbsoluteX;
    uint16_t lowByte = readMemory(PC++);
    uint16_t highByte = readMemory(PC++) << 8;
    opaddress = (highByte | lowByte) + X;
    opvalue = readMemory(opaddress);
}

void MOS6502::amABSY() {
    addressingMode = AbsoluteY;
    uint16_t lowByte = readMemory(PC++);
    uint16_t highByte = readMemory(PC++);
    opaddress = (highByte | lowByte) + Y;
    opvalue = readMemory(opaddress);
}

void MOS6502::amINDX() {
    addressingMode = IndexedIndirect;
    uint8_t lowByte = readMemory(PC++) + X;
    uint8_t highByte = lowByte + 1;
    opaddress = (highByte << 8) | lowByte;
    opvalue = readMemory(opaddress);
}

void MOS6502::amINDY() {
    addressingMode = IndirectIndexed;
    uint8_t lowByte = readMemory(PC++);
    lowByte = readMemory(0x0000 | lowByte);
    lowByte += Y;
    uint8_t carry = lowByte < Y ? 1 : 0;
    uint8_t highByte = readMemory(PC++) + carry;
    opaddress = (highByte << 8) | lowByte;
    opvalue = readMemory(opaddress);
}

// TODO: Even if it works I don't like it. Make it better. Horrible bit manipulation.
uint8_t MOS6502::opADC() {
    uint8_t carry = getFlag(Carry);
    uint16_t sumWithoutCarry = A + opvalue;
    uint16_t sum = sumWithoutCarry + carry;
    uint8_t sumLowByte = sum & 0xFF;
    setFlag(Carry, A > sumLowByte);
    setFlag(Zero, A == 0);
    bool positiveOverflow = ~(A & 0x80) && ~(opvalue & 0x80) && (sumWithoutCarry & 0x80);      // For two's complement: if the sum of two positive numbers is negative there's been an overflow
    bool negativeOverflow = (A & 0x80) && (opvalue & 0x80) && ~(sumWithoutCarry & 0x80);       // For two's complement: if the subtraction of two negative numbers is positive there's been an overflow
    if (carry && !positiveOverflow && !negativeOverflow) {
        positiveOverflow = ~(sumWithoutCarry & 0x80) && (sum & 0x80);
        negativeOverflow = (sumWithoutCarry & 0x80) && ~(sum & 0x80);
    }
    setFlag(Overflow, positiveOverflow || negativeOverflow);
    setFlag(Negative, sumLowByte & 0x80);
    A = sumLowByte;
    return A;
}

uint8_t MOS6502::opAND() {
    A &= opvalue;
    setFlag(Zero, A == 0);
    setFlag(Negative, A & 0x80);
    return A;
}

uint8_t MOS6502::opASL() {
    setFlag(Carry, opvalue & 0x80);
    A = opvalue << 1;
    setFlag(Zero, A == 0);
    setFlag(Negative, A & 0x80);
    return A;
}

uint8_t MOS6502::opBCC() {
    if (getFlag(Carry) == 0)
        PC += opvalue;
    return 0xFF;
}

uint8_t MOS6502::opBCS() {
    if (getFlag(Carry))
        PC += opvalue;
    return 0xFF;
}

uint8_t MOS6502::opBEQ() {
    if (getFlag(Zero))
        PC += opvalue;
    return 0xFF;
}

uint8_t MOS6502::opBIT() {
    uint8_t nonAccumulatedResult = A & opvalue;
    setFlag(Zero, nonAccumulatedResult == 0);
    setFlag(Overflow, nonAccumulatedResult & 0x40);
    setFlag(Negative, nonAccumulatedResult & 0x80);
    return nonAccumulatedResult;
}

uint8_t MOS6502::opBMI() {
    if (getFlag(Negative))
        PC += opvalue;
    return 0xFF;
}

uint8_t MOS6502::opBNE() {
    if (getFlag(Zero) == 0)
        PC += opvalue;
    return 0xFF;
}

uint8_t MOS6502::opBPL() {
    if (getFlag(Negative) == 0)
        PC += opvalue;
    return 0xFF;
}

uint8_t MOS6502::opBRK() {
    pushStack((PC & 0xFF00) >> 8);
    pushStack(PC & 0x00FF);
    pushStack(P);
    PC = 0xFFFF;
    setFlag(BreakCommand, true);
    return 0xFF;
}

uint8_t MOS6502::opBVC() {
    if (getFlag(Overflow) == 0)
        PC += opvalue;
    return 0xFF;
}

uint8_t MOS6502::opBVS() {
    if (getFlag(Overflow))
        PC += opvalue;
    return 0xFF;
}

uint8_t MOS6502::opCLC() {
    setFlag(Carry, false);
    return 0x00;
}

uint8_t MOS6502::opCLD() {
    setFlag(DecimalMode, false);
    return 0x00;
}

uint8_t MOS6502::opCLI() {
    setFlag(InterruptDisable, false);
    return 0x00;
}

uint8_t MOS6502::opCLV() {
    setFlag(Overflow, false);
    return 0x00;
}

uint8_t MOS6502::opCMP() {
    setFlag(Carry, A >= opvalue);
    setFlag(Zero, A == opvalue);
    uint8_t subtraction = A - opvalue;
    setFlag(Negative, subtraction & 0x80);
    return subtraction;
}

uint8_t MOS6502::opCPX() {
    setFlag(Carry, X >= opvalue);
    setFlag(Zero, X == opvalue);
    uint8_t subtraction = X - opvalue;
    setFlag(Negative, subtraction & 0x80);
    return subtraction;
}

uint8_t MOS6502::opCPY() {
    setFlag(Carry, Y >= opvalue);
    setFlag(Zero, Y == opvalue);
    uint8_t subtraction = Y - opvalue;
    setFlag(Negative, subtraction & 0x80);
    return subtraction;
}

uint8_t MOS6502::opDEC() {
    uint8_t result = opvalue - 1;
    setFlag(Zero, result == 0);
    setFlag(Negative, result & 0x80);
    writeMemory(opaddress, result);
    return result;
}

uint8_t MOS6502::opDEX() {
    X--;
    setFlag(Zero, X == 0);
    setFlag(Negative, X & 0x80);
    return X;
}

uint8_t MOS6502::opDEY() {
    Y--;
    setFlag(Zero, Y == 0);
    setFlag(Negative, Y & 0x80);
    return Y;
}

uint8_t MOS6502::opEOR() {
    A ^= opvalue;
    setFlag(Zero, A == 0);
    setFlag(Negative, A & 0x80);
    return A;
}

uint8_t MOS6502::opINC() {
    uint8_t result = opvalue + 1;
    setFlag(Zero, result == 0);
    setFlag(Negative, result & 0x80);
    writeMemory(opaddress, result);
    return result;
}

uint8_t MOS6502::opINX() {
    X++;
    setFlag(Zero, X == 0);
    setFlag(Negative, X & 0x80);
    return X;
}

uint8_t MOS6502::opINY() {
    Y++;
    setFlag(Zero, Y == 0);
    setFlag(Negative, Y & 0x80);
    return Y;
}

uint8_t MOS6502::opJMP() {
    PC = opaddress;
    return 0xFF;
}

uint8_t MOS6502::opJSR() {
    pushStack((PC & 0xFF00) >> 8);
    pushStack(PC & 0x00FF);
    PC = opaddress;
    return 0xFF;
}

uint8_t MOS6502::opLDA() {
    A = opvalue;
    setFlag(Negative, A & 0x80);
    setFlag(Zero, A == 0);
    return A;
}

uint8_t MOS6502::opLDX() {
    X = opvalue;
    setFlag(Negative, X & 0x80);
    setFlag(Zero, X == 0);
    return X;
}

uint8_t MOS6502::opLDY() {
    Y = opvalue;
    setFlag(Negative, Y & 0x80);
    setFlag(Zero, Y == 0);
    return Y;
}

uint8_t MOS6502::opLSR() {
    setFlag(Carry, opvalue & 0x01);
    A = opvalue >> 1;
    setFlag(Zero, A == 0);
    setFlag(Negative, A & 0x80);    // Can this be true?
    return A;
}

uint8_t MOS6502::opNOP() {
    return 0xFF;
}

uint8_t MOS6502::opORA() {
    A |= opvalue;
    setFlag(Zero, A == 0);
    setFlag(Negative, A & 0x80);
    return A;
}

uint8_t MOS6502::opPHA() {
    pushStack(A);
    return A;
}

uint8_t MOS6502::opPHP() {
    pushStack(P);
    return P;
}

uint8_t MOS6502::opPLA() {
    A = pullStack();
    setFlag(Zero, A == 0);
    setFlag(Negative, A & 0x80);
    return A;
}

uint8_t MOS6502::opPLP() {
    P = pullStack();
    return P;
}

uint8_t MOS6502::opROL() {
    bool hasCarry = opvalue & 0x80;
    A = (opvalue << 1) | getFlag(Carry);
    setFlag(Carry, hasCarry);
    setFlag(Zero, A == 0);
    setFlag(Negative, A & 0x80);
    return A;
}

uint8_t MOS6502::opROR() {
    bool hasCarry = opvalue & 0x01;
    A = (opvalue >> 1) | getFlag(Carry);
    setFlag(Carry, hasCarry);
    setFlag(Zero, A == 0);
    setFlag(Negative, A & 0x80);
    return A;
}

uint8_t MOS6502::opRTI() {
    P = pullStack();
    uint8_t PCL = pullStack();
    uint8_t PCH = pullStack();
    PC = (PCH << 8) | PCL;
    return 0xFF;
}

uint8_t MOS6502::opRTS() {
    uint8_t PCL = pullStack();
    uint8_t PCH = pullStack();
    PC = (PCH << 8) | PCL;
    return 0xFF;
}

uint8_t MOS6502::opSBC() {
    uint8_t carry = getFlag(Carry);
    uint16_t subtractionWithoutCarry = A - opvalue;
    uint16_t subtraction = subtractionWithoutCarry - carry;
    uint8_t subtractionLowByte = subtraction & 0x80;
    setFlag(Carry, A > subtractionLowByte);
    setFlag(Zero, A == 0);
    bool pnOverflow = ~(A & 0x80) && (opvalue & 0x80) && std::abs((int8_t) A - INT8_MIN) <= std::abs((int8_t) opvalue);
    bool npOverflow = (A & 0x80) && ~(opvalue & 0x80) && std::abs((int8_t) A - INT8_MAX) <= std::abs((int8_t) opvalue);
    if (carry && !npOverflow && !pnOverflow) {
        pnOverflow = ~(subtractionWithoutCarry & 0x80) && std::abs((int8_t) subtractionWithoutCarry - INT8_MIN) <= std::abs((int8_t) carry);
        npOverflow = (subtractionWithoutCarry & 0x80) && std::abs((int8_t) subtractionWithoutCarry - INT8_MAX) <= std::abs((int8_t) carry);
    }
    setFlag(Overflow, pnOverflow || npOverflow);
    setFlag(Negative, subtractionLowByte & 0x80);
    A = subtractionLowByte;
    return A;
}

uint8_t MOS6502::opSEC() {
    setFlag(Carry, true);
    return 0xFF;
}

uint8_t MOS6502::opSED() {
    setFlag(DecimalMode, true);
    return 0xFF;
}

uint8_t MOS6502::opSEI() {
    setFlag(InterruptDisable, true);
    return 0xFF;
}

uint8_t MOS6502::opSTA() {
    writeMemory(opaddress, A);
    return A;
}

uint8_t MOS6502::opSTX() {
    writeMemory(opaddress, X);
    return X;
}

uint8_t MOS6502::opSTY() {
    writeMemory(opaddress, Y);
    return Y;
}

uint8_t MOS6502::opTAX() {
    X = A;
    setFlag(Zero, X == 0);
    setFlag(Negative, X & 0x80);
    return X;
}

uint8_t MOS6502::opTAY() {
    Y = A;
    setFlag(Zero, Y == 0);
    setFlag(Negative, Y & 0x80);
    return Y;
}

uint8_t MOS6502::opTSX() {
    X = S;
    setFlag(Zero, X == 0);
    setFlag(Negative, X & 0x80);
    return X;
}

uint8_t MOS6502::opTXA() {
    A = X;
    setFlag(Zero, A == 0);
    setFlag(Negative, A & 0x80);
    return A;
}

uint8_t MOS6502::opTXS() {
    S = X;
    return S;
}

uint8_t MOS6502::opTYA() {
    A = Y;
    setFlag(Zero, A == 0);
    setFlag(Negative, A & 0x80);
    return A;
}
