//
// Created by junds on 04-09-2021.
//

#include "MOS6502.h"

void MOS6502::executeOperation(std::string const& operationAlias, VoidHandler addressingModeHandler, VoidHandler operationHandler, uint8_t baseCycles, bool canCrossPageBoundary, bool canBranch) {
    uint16_t previousPC = PC - 1;
    opalias = operationAlias;
    (this->*addressingModeHandler)();

    {
        // Update state before executing instruction
        preInstructionExecutionState.A = A;
        preInstructionExecutionState.X = X;
        preInstructionExecutionState.S = S;
        preInstructionExecutionState.P = P;
        preInstructionExecutionState.PC = PC - 1;
        preInstructionExecutionState.totalCycles = totalCyclesPerformed;
        preInstructionExecutionState.opcode = opcode;
        preInstructionExecutionState.operand = {};
        preInstructionExecutionState.instructionName = operationAlias;
    }

    logForNESTest(operationAlias, previousPC);

    (this->*operationHandler)();
    setFlag(Flag::B, true);
    opcycles = baseCycles + (canCrossPageBoundary && crossedPageBoundary) + (canBranch && isBranchTaken);
}

void MOS6502::handleUnknownOperation() const {
    logger->warn("The opcode {:0>2X} is not recognized, interpreting next byte as opcode", opcode);
}

void MOS6502::decodeOperation() {
    switch (opcode) {
        // ADC calls (add with carry)
        case 0x69: {
            executeOperation("ADC", &MOS6502::amIMM, &MOS6502::opADC, 2, false, false);
            break;
        }
        case 0x65: {
            executeOperation("ADC", &MOS6502::amZP, &MOS6502::opADC, 3, false, false);
            break;
        }
        case 0x75: {
            executeOperation("ADC", &MOS6502::amZPX, &MOS6502::opADC, 4, false, false);
            break;
        }
        case 0x6D: {
            executeOperation("ADC", &MOS6502::amABS, &MOS6502::opADC, 4, false, false);
            break;
        }
        case 0x7D: {
            executeOperation("ADC", &MOS6502::amABSX, &MOS6502::opADC, 4, true, false);
            break;
        }
        case 0x79: {
            executeOperation("ADC", &MOS6502::amABSY, &MOS6502::opADC, 4, true, false);
            break;
        }
        case 0x61: {
            executeOperation("ADC", &MOS6502::amINDX, &MOS6502::opADC, 6, false, false);
            break;
        }
        case 0x71: {
            executeOperation("ADC", &MOS6502::amINDY, &MOS6502::opADC, 5, true, false);
            break;
        }

        // AND calls (bitwise and)
        case 0x29: {
            executeOperation("AND", &MOS6502::amIMM, &MOS6502::opAND, 2, false, false);
            break;
        }
        case 0x25: {
            executeOperation("AND", &MOS6502::amZP, &MOS6502::opAND, 3, false, false);
            break;
        }
        case 0x35: {
            executeOperation("AND", &MOS6502::amZPX, &MOS6502::opAND, 4, false, false);
            break;
        }
        case 0x2D: {
            executeOperation("AND", &MOS6502::amABS, &MOS6502::opAND, 4, false, false);
            break;
        }
        case 0x3D: {
            executeOperation("AND", &MOS6502::amABSX, &MOS6502::opAND, 4, true, false);
            break;
        }
        case 0x39: {
            executeOperation("AND", &MOS6502::amABSY, &MOS6502::opAND, 4, true, false);
            break;
        }
        case 0x21: {
            executeOperation("AND", &MOS6502::amINDX, &MOS6502::opAND, 6, false, false);
            break;
        }
        case 0x31: {
            executeOperation("AND", &MOS6502::amINDY, &MOS6502::opAND, 5, true, false);
            break;
        }

        // ASL calls (arithmetic shift left)
        case 0x0A: {
            executeOperation("ASL", &MOS6502::amACC, &MOS6502::opASL, 2, false, false);
            break;
        }
        case 0x06: {
            executeOperation("ASL", &MOS6502::amZP, &MOS6502::opASL, 5, false, false);
            break;
        }
        case 0x16: {
            executeOperation("ASL", &MOS6502::amZPX, &MOS6502::opASL, 6, false, false);
            break;
        }
        case 0x0E: {
            executeOperation("ASL", &MOS6502::amABS, &MOS6502::opASL, 6, false, false);
            break;
        }
        case 0x1E: {
            executeOperation("ASL", &MOS6502::amABSX, &MOS6502::opASL, 7, false, false);
            break;
        }

        // BIT calls
        case 0x24: {
            executeOperation("BIT", &MOS6502::amZP, &MOS6502::opBIT, 3, false, false);
            break;
        }
        case 0x2C: {
            executeOperation("BIT", &MOS6502::amABS, &MOS6502::opBIT, 4, false, false);
            break;
        }

        // Branch calls
        case 0x10: {
            executeOperation("BPL", &MOS6502::amREL, &MOS6502::opBPL, 2, true, true);
            break;
        }
        case 0x30: {
            executeOperation("BMI", &MOS6502::amREL, &MOS6502::opBMI, 2, true, true);
            break;
        }
        case 0x50: {
            executeOperation("BVC", &MOS6502::amREL, &MOS6502::opBVC, 2, true, true);
            break;
        }
        case 0x70: {
            executeOperation("BVS", &MOS6502::amREL, &MOS6502::opBVS, 2, true, true);
            break;
        }
        case 0x90: {
            executeOperation("BCC", &MOS6502::amREL, &MOS6502::opBCC, 2, true, true);
            break;
        }
        case 0xB0: {
            executeOperation("BCS", &MOS6502::amREL, &MOS6502::opBCS, 2, true, true);
            break;
        }
        case 0xD0: {
            executeOperation("BNE", &MOS6502::amREL, &MOS6502::opBNE, 2, true, true);
            break;
        }
        case 0xF0: {
            executeOperation("BEQ", &MOS6502::amREL, &MOS6502::opBEQ, 2, true, true);
            break;
        }

        // BRK calls (break)
        case 0x00: {
            executeOperation("BRK", &MOS6502::amIMP, &MOS6502::opBRK, 7, false, false);
            break;
        }

        // CMP calls (compare accumulator)
        case 0xC9: {
            executeOperation("CMP", &MOS6502::amIMM, &MOS6502::opCMP, 2, false, false);
            break;
        }
        case 0xC5: {
            executeOperation("CMP", &MOS6502::amZP, &MOS6502::opCMP, 3, false, false);
            break;
        }
        case 0xD5: {
            executeOperation("CMP", &MOS6502::amZPX, &MOS6502::opCMP, 4, false, false);
            break;
        }
        case 0xCD: {
            executeOperation("CMP", &MOS6502::amABS, &MOS6502::opCMP, 4, false, false);
            break;
        }
        case 0xDD: {
            executeOperation("CMP", &MOS6502::amABSX, &MOS6502::opCMP, 4, true, false);
            break;
        }
        case 0xD9: {
            executeOperation("CMP", &MOS6502::amABSY, &MOS6502::opCMP, 4, true, false);
            break;
        }
        case 0xC1: {
            executeOperation("CMP", &MOS6502::amINDX, &MOS6502::opCMP, 6, false, false);
            break;
        }
        case 0xD1: {
            executeOperation("CMP", &MOS6502::amINDY, &MOS6502::opCMP, 5, true, false);
            break;
        }

        // CPX calls (compare X register)
        case 0xE0: {
            executeOperation("CPX", &MOS6502::amIMM, &MOS6502::opCPX, 2, false, false);
            break;
        }
        case 0xE4: {
            executeOperation("CPX", &MOS6502::amZP, &MOS6502::opCPX, 3, false, false);
            break;
        }
        case 0xEC: {
            executeOperation("CPX", &MOS6502::amABS, &MOS6502::opCPX, 4, false, false);
            break;
        }

        // CPY calls (compare Y register)
        case 0xC0: {
            executeOperation("CPY", &MOS6502::amIMM, &MOS6502::opCPY, 2, false, false);
            break;
        }
        case 0xC4: {
            executeOperation("CPY", &MOS6502::amZP, &MOS6502::opCPY, 3, false, false);
            break;
        }
        case 0xCC: {
            executeOperation("CPY", &MOS6502::amABS, &MOS6502::opCPY, 4, false, false);
            break;
        }

        // DEC calls (decrement)
        case 0xC6: {
            executeOperation("DEC", &MOS6502::amZP, &MOS6502::opDEC, 5, false, false);
            break;
        }
        case 0xD6: {
            executeOperation("DEC", &MOS6502::amZPX, &MOS6502::opDEC, 6, false, false);
            break;
        }
        case 0xCE: {
            executeOperation("DEC", &MOS6502::amABS, &MOS6502::opDEC, 6, false, false);
            break;
        }
        case 0xDE: {
            executeOperation("DEC", &MOS6502::amABSX, &MOS6502::opDEC, 7, false, false);
            break;
        }

        // EOR calls (bitwise exclusive or)
        case 0x49: {
            executeOperation("EOR", &MOS6502::amIMM, &MOS6502::opEOR, 2, false, false);
            break;
        }
        case 0x45: {
            executeOperation("EOR", &MOS6502::amZP, &MOS6502::opEOR, 3, false, false);
            break;
        }
        case 0x55: {
            executeOperation("EOR", &MOS6502::amZPX, &MOS6502::opEOR, 4, false, false);
            break;
        }
        case 0x4D: {
            executeOperation("EOR", &MOS6502::amABS, &MOS6502::opEOR, 4, false, false);
            break;
        }
        case 0x5D: {
            executeOperation("EOR", &MOS6502::amABSX, &MOS6502::opEOR, 4, true, false);
            break;
        }
        case 0x59: {
            executeOperation("EOR", &MOS6502::amABSY, &MOS6502::opEOR, 4, true, false);
            break;
        }
        case 0x41: {
            executeOperation("EOR", &MOS6502::amINDX, &MOS6502::opEOR, 6, false, false);
            break;
        }
        case 0x51: {
            executeOperation("EOR", &MOS6502::amINDY, &MOS6502::opEOR, 5, true, false);
            break;
        }

        // Flag instructions call (processor status)
        case 0x18: {
            executeOperation("CLC", &MOS6502::amIMP, &MOS6502::opCLC, 2, false, false);
            break;
        }
        case 0x38: {
            executeOperation("SEC", &MOS6502::amIMP, &MOS6502::opSEC, 2, false, false);
            break;
        }
        case 0x58: {
            executeOperation("CLI", &MOS6502::amIMP, &MOS6502::opCLI, 2, false, false);
            break;
        }
        case 0x78: {
            executeOperation("SEI", &MOS6502::amIMP, &MOS6502::opSEI, 2, false, false);
            break;
        }
        case 0xB8: {
            executeOperation("CLV", &MOS6502::amIMP, &MOS6502::opCLV, 2, false, false);
            break;
        }
        case 0xD8: {
            executeOperation("CLD", &MOS6502::amIMP, &MOS6502::opCLD, 2, false, false);
            break;
        }
        case 0xF8: {
            executeOperation("SED", &MOS6502::amIMP, &MOS6502::opSED, 2, false, false);
            break;
        }

        // INC calls (increment memory)
        case 0xE6: {
            executeOperation("INC", &MOS6502::amZP, &MOS6502::opINC, 5, false, false);
            break;
        }
        case 0xF6: {
            executeOperation("INC", &MOS6502::amZPX, &MOS6502::opINC, 6, false, false);
            break;
        }
        case 0xEE: {
            executeOperation("INC", &MOS6502::amABS, &MOS6502::opINC, 6, false, false);
            break;
        }
        case 0xFE: {
            executeOperation("INC", &MOS6502::amABSX, &MOS6502::opINC, 7, false, false);
            break;
        }

        // JMP calls (jump)
        case 0x4C: {
            executeOperation("JMP", &MOS6502::amABS, &MOS6502::opJMP, 3, false, false);
            break;
        }
        case 0x6C: {
            executeOperation("JMP", &MOS6502::amIND, &MOS6502::opJMP, 5, false, false);
            break;
        }

        // JSR calls (jump to subroutine)
        case 0x20: {
            executeOperation("JSR", &MOS6502::amABS, &MOS6502::opJSR, 6, false, false);
            break;
        }

        // LDA calls (load accumulator)
        case 0xA9: {
            executeOperation("LDA", &MOS6502::amIMM, &MOS6502::opLDA, 2, false, false);
            break;
        }
        case 0xA5: {
            executeOperation("LDA", &MOS6502::amZP, &MOS6502::opLDA, 3, false, false);
            break;
        }
        case 0xB5: {
            executeOperation("LDA", &MOS6502::amZPX, &MOS6502::opLDA, 4, false, false);
            break;
        }
        case 0xAD: {
            executeOperation("LDA", &MOS6502::amABS, &MOS6502::opLDA, 4, false, false);
            break;
        }
        case 0xBD: {
            executeOperation("LDA", &MOS6502::amABSX, &MOS6502::opLDA, 4, true, false);
            break;
        }
        case 0xB9: {
            executeOperation("LDA", &MOS6502::amABSY, &MOS6502::opLDA, 4, true, false);
            break;
        }
        case 0xA1: {
            executeOperation("LDA", &MOS6502::amINDX, &MOS6502::opLDA, 6, false, false);
            break;
        }
        case 0xB1: {
            executeOperation("LDA", &MOS6502::amINDY, &MOS6502::opLDA, 5, true, false);
            break;
        }

        // LDX calls (load X register)
        case 0xA2: {
            executeOperation("LDX", &MOS6502::amIMM, &MOS6502::opLDX, 2, false, false);
            break;
        }
        case 0xA6: {
            executeOperation("LDX", &MOS6502::amZP, &MOS6502::opLDX, 3, false, false);
            break;
        }
        case 0xB6: {
            executeOperation("LDX", &MOS6502::amZPY, &MOS6502::opLDX, 4, false, false);
            break;
        }
        case 0xAE: {
            executeOperation("LDX", &MOS6502::amABS, &MOS6502::opLDX, 4, false, false);
            break;
        }
        case 0xBE: {
            executeOperation("LDX", &MOS6502::amABSY, &MOS6502::opLDX, 4, true, false);
            break;
        }

        // LDY (load Y register)
        case 0xA0: {
            executeOperation("LDY", &MOS6502::amIMM, &MOS6502::opLDY, 2, false, false);
            break;
        }
        case 0xA4: {
            executeOperation("LDY", &MOS6502::amZP, &MOS6502::opLDY, 3, false, false);
            break;
        }
        case 0xB4: {
            executeOperation("LDY", &MOS6502::amZPX, &MOS6502::opLDY, 4, false, false);
            break;
        }
        case 0xAC: {
            executeOperation("LDY", &MOS6502::amABS, &MOS6502::opLDY, 4, false, false);
            break;
        }
        case 0xBC: {
            executeOperation("LDY", &MOS6502::amABSX, &MOS6502::opLDY, 4, true, false);
            break;
        }

        // LSR calls (logical shift right)
        case 0x4A: {
            executeOperation("LSR", &MOS6502::amACC, &MOS6502::opLSR, 2, false, false);
            break;
        }
        case 0x46: {
            executeOperation("LSR", &MOS6502::amZP, &MOS6502::opLSR, 5, false, false);
            break;
        }
        case 0x56: {
            executeOperation("LSR", &MOS6502::amZPX, &MOS6502::opLSR, 6, false, false);
            break;
        }
        case 0x4E: {
            executeOperation("LSR", &MOS6502::amABS, &MOS6502::opLSR, 6, false, false);
            break;
        }
        case 0x5E: {
            executeOperation("LSR", &MOS6502::amABSX, &MOS6502::opLSR, 7, false, false);
            break;
        }

        // NOP calls (no operation)
        case 0xEA: {
            executeOperation("NOP", &MOS6502::amIMP, &MOS6502::opNOP, 2, false, false);
            break;
        }

        // ORA calls (bitwise or with accumulator)
        case 0x09: {
            executeOperation("ORA", &MOS6502::amIMM, &MOS6502::opORA, 2, false, false);
            break;
        }
        case 0x05: {
            executeOperation("ORA", &MOS6502::amZP, &MOS6502::opORA, 3, false, false);
            break;
        }
        case 0x15: {
            executeOperation("ORA", &MOS6502::amZPX, &MOS6502::opORA, 4, false, false);
            break;
        }
        case 0x0D: {
            executeOperation("ORA", &MOS6502::amABS, &MOS6502::opORA, 4, false, false);
            break;
        }
        case 0x1D: {
            executeOperation("ORA", &MOS6502::amABSX, &MOS6502::opORA, 4, true, false);
            break;
        }
        case 0x19: {
            executeOperation("ORA", &MOS6502::amABSY, &MOS6502::opORA, 4, true, false);
            break;
        }
        case 0x01: {
            executeOperation("ORA", &MOS6502::amINDX, &MOS6502::opORA, 6, false, false);
            break;
        }
        case 0x11: {
            executeOperation("ORA", &MOS6502::amINDY, &MOS6502::opORA, 5, true, false);
            break;
        }

        // Register instructions calls
        case 0xAA: {
            executeOperation("TAX", &MOS6502::amIMP, &MOS6502::opTAX, 2, false, false);
            break;
        }
        case 0x8A: {
            executeOperation("TXA", &MOS6502::amIMP, &MOS6502::opTXA, 2, false, false);
            break;
        }
        case 0xCA: {
            executeOperation("DEX", &MOS6502::amIMP, &MOS6502::opDEX, 2, false, false);
            break;
        }
        case 0xE8: {
            executeOperation("INX", &MOS6502::amIMP, &MOS6502::opINX, 2, false, false);
            break;
        }
        case 0xA8: {
            executeOperation("TAY", &MOS6502::amIMP, &MOS6502::opTAY, 2, false, false);
            break;
        }
        case 0x98: {
            executeOperation("TYA", &MOS6502::amIMP, &MOS6502::opTYA, 2, false, false);
            break;
        }
        case 0x88: {
            executeOperation("DEY", &MOS6502::amIMP, &MOS6502::opDEY, 2, false, false);
            break;
        }
        case 0xC8: {
            executeOperation("INY", &MOS6502::amIMP, &MOS6502::opINY, 2, false, false);
            break;
        }

        // ROL calls (rotate left)
        case 0x2A: {
            executeOperation("ROL", &MOS6502::amACC, &MOS6502::opROL, 2, false, false);
            break;
        }
        case 0x26: {
            executeOperation("ROL", &MOS6502::amZP, &MOS6502::opROL, 5, false, false);
            break;
        }
        case 0x36: {
            executeOperation("ROL", &MOS6502::amZPX, &MOS6502::opROL, 6, false, false);
            break;
        }
        case 0x2E: {
            executeOperation("ROL", &MOS6502::amABS, &MOS6502::opROL, 6, false, false);
            break;
        }
        case 0x3E: {
            executeOperation("ROL", &MOS6502::amABSX, &MOS6502::opROL, 7, false, false);
            break;
        }

        // ROR calls (rotate right)
        case 0x6A: {
            executeOperation("ROR", &MOS6502::amACC, &MOS6502::opROR, 2, false, false);
            break;
        }
        case 0x66: {
            executeOperation("ROR", &MOS6502::amZP, &MOS6502::opROR, 5, false, false);
            break;
        }
        case 0x76: {
            executeOperation("ROR", &MOS6502::amZPX, &MOS6502::opROR, 6, false, false);
            break;
        }
        case 0x6E: {
            executeOperation("ROR", &MOS6502::amABS, &MOS6502::opROR, 6, false, false);
            break;
        }
        case 0x7E: {
            executeOperation("ROR", &MOS6502::amABSX, &MOS6502::opROR, 7, false, false);
            break;
        }

        // RTI calls (return from interrupt)
        case 0x40: {
            executeOperation("RTI", &MOS6502::amIMP, &MOS6502::opRTI, 6, false, false);
            break;
        }

        // RTS calls (return from subroutine)
        case 0x60: {
            executeOperation("RTS", &MOS6502::amIMP, &MOS6502::opRTS, 6, false, false);
            break;
        }

        // SBC calls (subtract with carry)
        case 0xE9: {
            executeOperation("SBC", &MOS6502::amIMM, &MOS6502::opSBC, 2, false, false);
            break;
        }
        case 0xE5: {
            executeOperation("SBC", &MOS6502::amZP, &MOS6502::opSBC, 3, false, false);
            break;
        }
        case 0xF5: {
            executeOperation("SBC", &MOS6502::amZPX, &MOS6502::opSBC, 4, false, false);
            break;
        }
        case 0xED: {
            executeOperation("SBC", &MOS6502::amABS, &MOS6502::opSBC, 4, false, false);
            break;
        }
        case 0xFD: {
            executeOperation("SBC", &MOS6502::amABSX, &MOS6502::opSBC, 4, true, false);
            break;
        }
        case 0xF9: {
            executeOperation("SBC", &MOS6502::amABSY, &MOS6502::opSBC, 4, true, false);
            break;
        }
        case 0xE1: {
            executeOperation("SBC", &MOS6502::amINDX, &MOS6502::opSBC, 6, false, false);
            break;
        }
        case 0xF1: {
            executeOperation("SBC", &MOS6502::amINDY, &MOS6502::opSBC, 5, true, false);
            break;
        }

        // STA calls (store accumulator)
        case 0x85: {
            executeOperation("STA", &MOS6502::amZP, &MOS6502::opSTA, 3, false, false);
            break;
        }
        case 0x95: {
            executeOperation("STA", &MOS6502::amZPX, &MOS6502::opSTA, 4, false, false);
            break;
        }
        case 0x8D: {
            executeOperation("STA", &MOS6502::amABS, &MOS6502::opSTA, 4, false, false);
            break;
        }
        case 0x9D: {
            executeOperation("STA", &MOS6502::amABSX, &MOS6502::opSTA, 5, false, false);
            break;
        }
        case 0x99: {
            executeOperation("STA", &MOS6502::amABSY, &MOS6502::opSTA, 5, false, false);
            break;
        }
        case 0x81: {
            executeOperation("STA", &MOS6502::amINDX, &MOS6502::opSTA, 6, false, false);
            break;
        }
        case 0x91: {
            executeOperation("STA", &MOS6502::amINDY, &MOS6502::opSTA, 6, false, false);
            break;
        }

        // Stack instructions calls
        case 0x9A: {
            executeOperation("TXS", &MOS6502::amIMP, &MOS6502::opTXS, 2, false, false);
            break;
        }
        case 0xBA: {
            executeOperation("TSX", &MOS6502::amIMP, &MOS6502::opTSX, 2, false, false);
            break;
        }
        case 0x48: {
            executeOperation("PHA", &MOS6502::amIMP, &MOS6502::opPHA, 3, false, false);
            break;
        }
        case 0x68: {
            executeOperation("PLA", &MOS6502::amIMP, &MOS6502::opPLA, 4, false, false);
            break;
        }
        case 0x08: {
            executeOperation("PHP", &MOS6502::amIMP, &MOS6502::opPHP, 3, false, false);
            break;
        }
        case 0x28: {
            executeOperation("PLP", &MOS6502::amIMP, &MOS6502::opPLP, 4, false, false);
            break;
        }

        // STX calls (store X register)
        case 0x86: {
            executeOperation("STX", &MOS6502::amZP, &MOS6502::opSTX, 3, false, false);
            break;
        }
        case 0x96: {
            executeOperation("STX", &MOS6502::amZPY, &MOS6502::opSTX, 4, false, false);
            break;
        }
        case 0x8E: {
            executeOperation("STX", &MOS6502::amABS, &MOS6502::opSTX, 4, false, false);
            break;
        }

        // STY calls (store Y register)
        case 0x84: {
            executeOperation("STY", &MOS6502::amZP, &MOS6502::opSTY, 3, false, false);
            break;
        }
        case 0x94: {
            executeOperation("STY", &MOS6502::amZPX, &MOS6502::opSTY, 4, false, false);
            break;
        }
        case 0x8C: {
            executeOperation("STY", &MOS6502::amABS, &MOS6502::opSTY, 4, false, false);
            break;
        }

        default: handleIllegalOperation();
    }
}

void MOS6502::handleIllegalOperation() {
    switch (opcode) {
        case 0x04:
        case 0x44:
        case 0x64: {
            executeOperation("NOP", &MOS6502::amZP, &MOS6502::opNOP, 3, false, false);
            break;
        }
        case 0x0C: {
            executeOperation("NOP", &MOS6502::amABS, &MOS6502::opNOP, 4, false, false);
            break;
        }
        case 0x14:
        case 0x34:
        case 0x54:
        case 0x74:
        case 0xD4:
        case 0xF4: {
            executeOperation("NOP", &MOS6502::amZPX, &MOS6502::opNOP, 4, false, false);
            break;
        }
        case 0x1A:
        case 0x3A:
        case 0x5A:
        case 0x7A:
        case 0xDA:
        case 0xFA: {
            executeOperation("NOP", &MOS6502::amIMP, &MOS6502::opNOP, 2, false, false);
            break;
        }
        case 0x80:
        case 0x82:
        case 0x89:
        case 0xC2:
        case 0xE2: {
            executeOperation("NOP", &MOS6502::amIMM, &MOS6502::opNOP, 2, false, false);
            break;
        }
        case 0x1C:
        case 0x3C:
        case 0x5C:
        case 0x7C:
        case 0xDC:
        case 0xFC: {
            executeOperation("NOP", &MOS6502::amABSX, &MOS6502::opNOP, 4, true, false);
            break;
        }
        case 0xA3: {
            executeOperation("LAX", &MOS6502::amINDX, &MOS6502::opLAX, 6, false, false);
            break;
        }
        case 0xA7: {
            executeOperation("LAX", &MOS6502::amZP, &MOS6502::opLAX, 3, false, false);
            break;
        }
        case 0xAF: {
            executeOperation("LAX", &MOS6502::amABS, &MOS6502::opLAX, 4, false, false);
            break;
        }
        case 0xB3: {
            executeOperation("LAX", &MOS6502::amINDY, &MOS6502::opLAX, 5, true, false);
            break;
        }
        case 0xB7: {
            executeOperation("LAX", &MOS6502::amZPY, &MOS6502::opLAX, 4, false, false);
            break;
        }
        case 0xBF: {
            executeOperation("LAX", &MOS6502::amABSY, &MOS6502::opLAX, 4, true, false);
            break;
        }
        case 0x83: {
            executeOperation("SAX", &MOS6502::amINDX, &MOS6502::opSAX, 6, false, false);
            break;
        }
        case 0x87: {
            executeOperation("SAX", &MOS6502::amZP, &MOS6502::opSAX, 3, false, false);
            break;
        }
        case 0x8F: {
            executeOperation("SAX", &MOS6502::amABS, &MOS6502::opSAX, 4, false, false);
            break;
        }
        case 0x97: {
            executeOperation("SAX", &MOS6502::amZPY, &MOS6502::opSAX, 4, false, false);
            break;
        }
        case 0xEB: {
            executeOperation("SBC", &MOS6502::amIMM, &MOS6502::opSBC, 2, false, false); // It's the same as the "legal" opcode $E9
            break;
        }
        case 0xC3: {
            executeOperation("DCP", &MOS6502::amINDX, &MOS6502::opDCP, 8, false, false);
            break;
        }
        case 0xC7: {
            executeOperation("DCP", &MOS6502::amZP, &MOS6502::opDCP, 5, false, false);
            break;
        }
        case 0xCF: {
            executeOperation("DCP", &MOS6502::amABS, &MOS6502::opDCP, 6, false, false);
            break;
        }
        case 0xD3: {
            executeOperation("DCP", &MOS6502::amINDY, &MOS6502::opDCP, 8, false, false);
            break;
        }
        case 0xD7: {
            executeOperation("DCP", &MOS6502::amZPX, &MOS6502::opDCP, 6, false, false);
            break;
        }
        case 0xDB: {
            executeOperation("DCP", &MOS6502::amABSY, &MOS6502::opDCP, 7, false, false);
            break;
        }
        case 0xDF: {
            executeOperation("DCP", &MOS6502::amABSX, &MOS6502::opDCP, 7, false, false);
            break;
        }
        case 0xE3: {
            executeOperation("ISB", &MOS6502::amINDX, &MOS6502::opISB, 8, false, false);
            break;
        }
        case 0xE7: {
            executeOperation("ISB", &MOS6502::amZP, &MOS6502::opISB, 5, false, false);
            break;
        }
        case 0xEF: {
            executeOperation("ISB", &MOS6502::amABS, &MOS6502::opISB, 6, false, false);
            break;
        }
        case 0xF3: {
            executeOperation("ISB", &MOS6502::amINDY, &MOS6502::opISB, 8, false, false);
            break;
        }
        case 0xF7: {
            executeOperation("ISB", &MOS6502::amZPX, &MOS6502::opISB, 6, false, false);
            break;
        }
        case 0xFB: {
            executeOperation("ISB", &MOS6502::amABSY, &MOS6502::opISB, 7, false, false);
            break;
        }
        case 0xFF: {
            executeOperation("ISB", &MOS6502::amABSX, &MOS6502::opISB, 7, false, false);
            break;
        }
        case 0x03: {
            executeOperation("SLO", &MOS6502::amINDX, &MOS6502::opSLO, 8, false, false);
            break;
        }
        case 0x07: {
            executeOperation("SLO", &MOS6502::amZP, &MOS6502::opSLO, 5, false, false);
            break;
        }
        case 0x0F: {
            executeOperation("SLO", &MOS6502::amABS, &MOS6502::opSLO, 6, false, false);
            break;
        }
        case 0x13: {
            executeOperation("SLO", &MOS6502::amINDY, &MOS6502::opSLO, 8, false, false);
            break;
        }
        case 0x17: {
            executeOperation("SLO", &MOS6502::amZPX, &MOS6502::opSLO, 6, false, false);
            break;
        }
        case 0x1B: {
            executeOperation("SLO", &MOS6502::amABSY, &MOS6502::opSLO, 7, false, false);
            break;
        }
        case 0x1F: {
            executeOperation("SLO", &MOS6502::amABSX, &MOS6502::opSLO, 7, false, false);
            break;
        }
        case 0x23: {
            executeOperation("RLA", &MOS6502::amINDX, &MOS6502::opRLA, 8, false, false);
            break;
        }
        case 0x27: {
            executeOperation("RLA", &MOS6502::amZP, &MOS6502::opRLA, 5, false, false);
            break;
        }
        case 0x2F: {
            executeOperation("RLA", &MOS6502::amABS, &MOS6502::opRLA, 6, false, false);
            break;
        }
        case 0x33: {
            executeOperation("RLA", &MOS6502::amINDY, &MOS6502::opRLA, 8, false, false);
            break;
        }
        case 0x37: {
            executeOperation("RLA", &MOS6502::amZPX, &MOS6502::opRLA, 6, false, false);
            break;
        }
        case 0x3B: {
            executeOperation("RLA", &MOS6502::amABSY, &MOS6502::opRLA, 7, false, false);
            break;
        }
        case 0x3F: {
            executeOperation("RLA", &MOS6502::amABSX, &MOS6502::opRLA, 7, false, false);
            break;
        }
        case 0x43: {
            executeOperation("SRE", &MOS6502::amINDX, &MOS6502::opSRE, 8, false, false);
            break;
        }
        case 0x47: {
            executeOperation("SRE", &MOS6502::amZP, &MOS6502::opSRE, 5, false, false);
            break;
        }
        case 0x4F: {
            executeOperation("SRE", &MOS6502::amABS, &MOS6502::opSRE, 6, false, false);
            break;
        }
        case 0x53: {
            executeOperation("SRE", &MOS6502::amINDY, &MOS6502::opSRE, 8, false, false);
            break;
        }
        case 0x57: {
            executeOperation("SRE", &MOS6502::amZPX, &MOS6502::opSRE, 6, false, false);
            break;
        }
        case 0x5B: {
            executeOperation("SRE", &MOS6502::amABSY, &MOS6502::opSRE, 7, false, false);
            break;
        }
        case 0x5F: {
            executeOperation("SRE", &MOS6502::amABSX, &MOS6502::opSRE, 7, false, false);
            break;
        }
        case 0x63: {
            executeOperation("RRA", &MOS6502::amINDX, &MOS6502::opRRA, 8, false, false);
            break;
        }
        case 0x67: {
            executeOperation("RRA", &MOS6502::amZP, &MOS6502::opRRA, 5, false, false);
            break;
        }
        case 0x6F: {
            executeOperation("RRA", &MOS6502::amABS, &MOS6502::opRRA, 6, false, false);
            break;
        }
        case 0x73: {
            executeOperation("RRA", &MOS6502::amINDY, &MOS6502::opRRA, 8, false, false);
            break;
        }
        case 0x77: {
            executeOperation("RRA", &MOS6502::amZPX, &MOS6502::opRRA, 6, false, false);
            break;
        }
        case 0x7B: {
            executeOperation("RRA", &MOS6502::amABSY, &MOS6502::opRRA, 7, false, false);
            break;
        }
        case 0x7F: {
            executeOperation("RRA", &MOS6502::amABSX, &MOS6502::opRRA, 7, false, false);
            break;
        }
        default: handleUnknownOperation();
    }
}
