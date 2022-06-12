//
// Created by junds on 19-09-2021.
//

#include "MOS6502.h"

// TODO: Even if it works I don't like it. Make it better. Horrible bit manipulation.
void MOS6502::opADC() {
    uint8_t carry = getFlag(Flag::Carry);
    uint16_t sum = A + opvalue;
    bool hasCarry = sum > 0x00FF;
    bool operandsShareSign = (A & 0x80) == (opvalue & 0x80);    // For two's complement: if the sum's operands share sign and the result has a different sign there's been an overflow
    bool sumHasDifferentSign = (A & 0x80) ^ (sum & 0x0080);
    bool overflow = operandsShareSign && sumHasDifferentSign;

    if (carry) {
        operandsShareSign = (sum & 0x0080) == 0x00;
        sum += carry;
        sumHasDifferentSign = sum & 0x0080;
        overflow = overflow || (operandsShareSign && sumHasDifferentSign);
        hasCarry = sum > 0x00FF;
    }

    A = sum & 0x00FF;
    setFlag(Flag::Carry, hasCarry);
    setFlag(Flag::Zero, A == 0);
    setFlag(Flag::Overflow, overflow);
    setFlag(Flag::Negative, A & 0x80);
}

void MOS6502::opAND() {
    A &= opvalue;
    setFlag(Flag::Zero, A == 0);
    setFlag(Flag::Negative, A & 0x80);
}

void MOS6502::opASL() {
    setFlag(Flag::Carry, opvalue & 0x80);
    opvalue = opvalue << 1;
    setFlag(Flag::Zero, opvalue == 0);
    setFlag(Flag::Negative, opvalue & 0x80);
    if (addressingMode == AddressingMode::Accumulator)
        A = opvalue;
    else
        writeMemory(opaddress, opvalue);
}

void MOS6502::opBCC() {
    isBranchTaken = getFlag(Flag::Carry) == 0;
    crossedPageBoundary = crossedPageBoundary && isBranchTaken;
    if (isBranchTaken)
        PC = opaddress;
}

void MOS6502::opBCS() {
    isBranchTaken = getFlag(Flag::Carry);
    crossedPageBoundary = crossedPageBoundary && isBranchTaken;
    if (isBranchTaken)
        PC = opaddress;
}

void MOS6502::opBEQ() {
    isBranchTaken = getFlag(Flag::Zero);
    crossedPageBoundary = crossedPageBoundary && isBranchTaken;
    if (isBranchTaken)
        PC = opaddress;
}

void MOS6502::opBIT() {
    setFlag(Flag::Zero, (opvalue & A) == 0);
    setFlag(Flag::Overflow, opvalue & 0x40);
    setFlag(Flag::Negative, opvalue & 0x80);
}

void MOS6502::opBMI() {
    isBranchTaken = getFlag(Flag::Negative);
    crossedPageBoundary = crossedPageBoundary && isBranchTaken;
    if (isBranchTaken)
        PC = opaddress;
}

void MOS6502::opBNE() {
    isBranchTaken = getFlag(Flag::Zero) == 0;
    crossedPageBoundary = crossedPageBoundary && isBranchTaken;
    if (isBranchTaken)
        PC = opaddress;
}

void MOS6502::opBPL() {
    isBranchTaken = getFlag(Flag::Negative) == 0;
    crossedPageBoundary = crossedPageBoundary && isBranchTaken;
    if (isBranchTaken)
        PC = opaddress;
}

void MOS6502::opBRK() {
    PC++;
    uint8_t preservedP = P;
    preservedP |= (1 << 4);
    preservedP |= (1 << 5);
    interrupt(0xFFFE, 0xFFFF, preservedP);
    setFlag(Flag::BreakCommand, true);
}

void MOS6502::opBVC() {
    isBranchTaken = getFlag(Flag::Overflow) == 0;
    crossedPageBoundary = crossedPageBoundary && isBranchTaken;
    if (isBranchTaken)
        PC = opaddress;
}

void MOS6502::opBVS() {
    isBranchTaken = getFlag(Flag::Overflow);
    crossedPageBoundary = crossedPageBoundary && isBranchTaken;
    if (isBranchTaken)
        PC = opaddress;
}

void MOS6502::opCLC() {
    setFlag(Flag::Carry, false);
}

void MOS6502::opCLD() {
    setFlag(Flag::DecimalMode, false);
}

void MOS6502::opCLI() {
    setFlag(Flag::InterruptDisable, false);
}

void MOS6502::opCLV() {
    setFlag(Flag::Overflow, false);
}

void MOS6502::opCMP() {
    setFlag(Flag::Carry, A >= opvalue);
    setFlag(Flag::Zero, A == opvalue);
    uint8_t subtraction = A - opvalue;
    setFlag(Flag::Negative, subtraction & 0x80);
}

void MOS6502::opCPX() {
    setFlag(Flag::Carry, X >= opvalue);
    setFlag(Flag::Zero, X == opvalue);
    uint8_t subtraction = X - opvalue;
    setFlag(Flag::Negative, subtraction & 0x80);
}

void MOS6502::opCPY() {
    setFlag(Flag::Carry, Y >= opvalue);
    setFlag(Flag::Zero, Y == opvalue);
    uint8_t subtraction = Y - opvalue;
    setFlag(Flag::Negative, subtraction & 0x80);
}

void MOS6502::opDEC() {
    opvalue = opvalue - 1;
    setFlag(Flag::Zero, opvalue == 0);
    setFlag(Flag::Negative, opvalue & 0x80);
    writeMemory(opaddress, opvalue);
}

void MOS6502::opDEX() {
    X--;
    setFlag(Flag::Zero, X == 0);
    setFlag(Flag::Negative, X & 0x80);
}

void MOS6502::opDEY() {
    Y--;
    setFlag(Flag::Zero, Y == 0);
    setFlag(Flag::Negative, Y & 0x80);
}

void MOS6502::opEOR() {
    A ^= opvalue;
    setFlag(Flag::Zero, A == 0);
    setFlag(Flag::Negative, A & 0x80);
}

void MOS6502::opINC() {
    opvalue = opvalue + 1;
    setFlag(Flag::Zero, opvalue == 0);
    setFlag(Flag::Negative, opvalue & 0x80);
    writeMemory(opaddress, opvalue);
}

void MOS6502::opINX() {
    X++;
    setFlag(Flag::Zero, X == 0);
    setFlag(Flag::Negative, X & 0x80);
}

void MOS6502::opINY() {
    Y++;
    setFlag(Flag::Zero, Y == 0);
    setFlag(Flag::Negative, Y & 0x80);
}

void MOS6502::opJMP() {
    PC = opaddress;
}

void MOS6502::opJSR() {
    uint16_t previousPC = PC - 1;
    pushStack((previousPC & 0xFF00) >> 8);
    pushStack(previousPC & 0x00FF);
    PC = opaddress;
}

void MOS6502::opLDA() {
    A = opvalue;
    setFlag(Flag::Negative, A & 0x80);
    setFlag(Flag::Zero, A == 0);
}

void MOS6502::opLDX() {
    X = opvalue;
    setFlag(Flag::Negative, X & 0x80);
    setFlag(Flag::Zero, X == 0);
}

void MOS6502::opLDY() {
    Y = opvalue;
    setFlag(Flag::Negative, Y & 0x80);
    setFlag(Flag::Zero, Y == 0);
}

void MOS6502::opLSR() {
    setFlag(Flag::Carry, opvalue & 0x01);
    opvalue = opvalue >> 1;
    setFlag(Flag::Zero, opvalue == 0);
    setFlag(Flag::Negative, opvalue & 0x80);    // Can this be true?
    if (addressingMode == AddressingMode::Accumulator)
        A = opvalue;
    else
        writeMemory(opaddress, opvalue);
}

void MOS6502::opNOP() {
}

void MOS6502::opORA() {
    A |= opvalue;
    setFlag(Flag::Zero, A == 0);
    setFlag(Flag::Negative, A & 0x80);
}

void MOS6502::opPHA() {
    pushStack(A);
}

void MOS6502::opPHP() {
    uint8_t pushP = P;
    pushP |= (1 << 4);
    pushP |= (1 << 5);
    pushStack(pushP);
}

void MOS6502::opPLA() {
    A = pullStack();
    setFlag(Flag::Zero, A == 0);
    setFlag(Flag::Negative, A & 0x80);
}

// FIXME: This is so hackish I don't like it, think of a better way to handle flags BreakCommand and B (bit 4 and 5)
void MOS6502::opPLP() {
    uint8_t newP = pullStack();
    newP &= ~(1 << 4);  // The NES CPU doesn't have storage for the bit 4 (BreakCommand flag, it's only generated when pushing)
    P = newP;
}

void MOS6502::opROL() {
    bool hasCarry = opvalue & 0x80;
    opvalue = (opvalue << 1) | getFlag(Flag::Carry);
    setFlag(Flag::Carry, hasCarry);
    setFlag(Flag::Zero, opvalue == 0);
    setFlag(Flag::Negative, opvalue & 0x80);
    if (addressingMode == AddressingMode::Accumulator)
        A = opvalue;
    else
        writeMemory(opaddress, opvalue);
}

void MOS6502::opROR() {
    bool hasCarry = opvalue & 0x01;
    uint8_t previousCarry = getFlag(Flag::Carry) ? 1 : 0;
    opvalue = (opvalue >> 1) | (previousCarry << 7);
    setFlag(Flag::Carry, hasCarry);
    setFlag(Flag::Zero, opvalue == 0);
    setFlag(Flag::Negative, opvalue & 0x80);
    if (addressingMode == AddressingMode::Accumulator)
        A = opvalue;
    else
        writeMemory(opaddress, opvalue);
}

void MOS6502::opRTI() {
    P = pullStack();
    uint8_t PCL = pullStack();
    uint8_t PCH = pullStack();
    PC = (PCH << 8) | PCL;
}

void MOS6502::opRTS() {
    uint8_t PCL = pullStack();
    uint8_t PCH = pullStack();
    PC = ((PCH << 8) | PCL) + 1;    // It pulls the PC minus 1, see JSR
}

void MOS6502::opSBC() {
    opvalue = ~opvalue;
    opADC();
}

void MOS6502::opSEC() {
    setFlag(Flag::Carry, true);
}

void MOS6502::opSED() {
    setFlag(Flag::DecimalMode, true);
}

void MOS6502::opSEI() {
    setFlag(Flag::InterruptDisable, true);
}

void MOS6502::opSTA() {
    writeMemory(opaddress, A);
}

void MOS6502::opSTX() {
    writeMemory(opaddress, X);
}

void MOS6502::opSTY() {
    writeMemory(opaddress, Y);
}

void MOS6502::opTAX() {
    X = A;
    setFlag(Flag::Zero, X == 0);
    setFlag(Flag::Negative, X & 0x80);
}

void MOS6502::opTAY() {
    Y = A;
    setFlag(Flag::Zero, Y == 0);
    setFlag(Flag::Negative, Y & 0x80);
}

void MOS6502::opTSX() {
    X = S;
    setFlag(Flag::Zero, X == 0);
    setFlag(Flag::Negative, X & 0x80);
}

void MOS6502::opTXA() {
    A = X;
    setFlag(Flag::Zero, A == 0);
    setFlag(Flag::Negative, A & 0x80);
}

void MOS6502::opTXS() {
    S = X;
}

void MOS6502::opTYA() {
    A = Y;
    setFlag(Flag::Zero, A == 0);
    setFlag(Flag::Negative, A & 0x80);
}

void MOS6502::opLAX() {
    opLDA();
    opLDX();
}

void MOS6502::opSAX() {
    writeMemory(opaddress, X & A);
}

void MOS6502::opDCP() {
    opDEC();
    opCMP();
}

void MOS6502::opISB() {
    opINC();
    opSBC();
}

void MOS6502::opSLO() {
    opASL();
    opORA();
}

void MOS6502::opRLA() {
    opROL();
    opAND();
}

void MOS6502::opSRE() {
    opLSR();
    opEOR();
}

void MOS6502::opRRA() {
    opROR();
    opADC();
}
