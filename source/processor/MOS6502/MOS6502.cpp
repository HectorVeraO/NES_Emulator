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

uint8_t MOS6502::opADC() {

}

uint8_t MOS6502::opAND() {

}

uint8_t MOS6502::opASL() {

}

uint8_t MOS6502::opBCC() {

}

uint8_t MOS6502::opBCS() {

}

uint8_t MOS6502::opBEQ() {

}

uint8_t MOS6502::opBIT() {

}

uint8_t MOS6502::opBMI() {

}

uint8_t MOS6502::opBNE() {

}

uint8_t MOS6502::opBPL() {

}

uint8_t MOS6502::opBRK() {

}

uint8_t MOS6502::opBVC() {

}

uint8_t MOS6502::opBVS() {

}

uint8_t MOS6502::opCLC() {

}

uint8_t MOS6502::opCLD() {

}

uint8_t MOS6502::opCLI() {

}

uint8_t MOS6502::opCLV() {

}

uint8_t MOS6502::opCMP() {

}

uint8_t MOS6502::opCPX() {

}

uint8_t MOS6502::opCPY() {

}

uint8_t MOS6502::opDEC() {

}

uint8_t MOS6502::opDEX() {

}

uint8_t MOS6502::opDEY() {

}

uint8_t MOS6502::opEOR() {

}

uint8_t MOS6502::opINC() {

}

uint8_t MOS6502::opINX() {

}

uint8_t MOS6502::opINY() {

}

uint8_t MOS6502::opJMP() {

}

uint8_t MOS6502::opJSR() {

}

uint8_t MOS6502::opLDA() {
    A = opvalue;
    setFlag(Negative, (A & 0x80) >> 7);
    setFlag(Zero, A == 0);
    return A;
}

uint8_t MOS6502::opLDX() {
    X = opvalue;
    setFlag(Negative, (X & 0x80) >> 7);
    setFlag(Zero, X == 0);
    return X;
}

uint8_t MOS6502::opLDY() {
    Y = opvalue;
    setFlag(Negative, (Y & 0x80) >> 7);
    setFlag(Zero, Y == 0);
    return Y;
}

uint8_t MOS6502::opLSR() {

}

uint8_t MOS6502::opNOP() {

}

uint8_t MOS6502::opORA() {

}

uint8_t MOS6502::opPHA() {

}

uint8_t MOS6502::opPHP() {

}

uint8_t MOS6502::opPLA() {

}

uint8_t MOS6502::opPLP() {

}

uint8_t MOS6502::opROL() {

}

uint8_t MOS6502::opROR() {

}

uint8_t MOS6502::opRTI() {

}

uint8_t MOS6502::opRTS() {

}

uint8_t MOS6502::opSBC() {

}

uint8_t MOS6502::opSEC() {

}

uint8_t MOS6502::opSED() {

}

uint8_t MOS6502::opSEI() {

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

}

uint8_t MOS6502::opTXA() {
    A = X;
    setFlag(Zero, A == 0);
    setFlag(Negative, A & 0x80);
    return A;
}

uint8_t MOS6502::opTXS() {

}

uint8_t MOS6502::opTYA() {
    A = Y;
    setFlag(Zero, A == 0);
    setFlag(Negative, A & 0x80);
    return A;
}
