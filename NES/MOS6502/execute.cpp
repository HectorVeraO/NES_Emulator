//
// Created by junds on 04-09-2021.
//

#include "MOS6502.h"

void MOS6502::executeOperation() {
    switch (opcode) {
        // ADC calls (add with carry)
        case 0x69: {
            amIMM();
            opADC();
            opcycles += 2;
            break;
        }
        case 0x65: {
            amZP();
            opADC();
            opcycles += 3;
            break;
        }
        case 0x75: {
            amZPX();
            opADC();
            opcycles += 4;
            break;
        }
        case 0x6D: {
            amABS();
            opADC();
            opcycles += 4;
            break;
        }
        case 0x7D: {
            amABSX();
            opADC();
            opcycles += 4 + crossedPageBoundary;
            break;
        }
        case 0x79: {
            amABSY();
            opADC();
            opcycles += 4 + crossedPageBoundary;
            break;
        }
        case 0x61: {
            amINDX();
            opADC();
            opcycles += 6;
            break;
        }
        case 0x71: {
            amINDY();
            opADC();
            opcycles += 5 + crossedPageBoundary;
            break;
        }

        // AND calls (bitwise and)
        case 0x29: {
            amIMM();
            opAND();
            opcycles += 2;
            break;
        }
        case 0x25: {
            amZP();
            opAND();
            opcycles += 3;
            break;
        }
        case 0x35: {
            amZPX();
            opAND();
            opcycles += 4;
            break;
        }
        case 0x2D: {
            amABS();
            opAND();
            opcycles += 4;
            break;
        }
        case 0x3D: {
            amABSX();
            opAND();
            opcycles += 4 + crossedPageBoundary;
            break;
        }
        case 0x39: {
            amABSY();
            opAND();
            opcycles += 4 + crossedPageBoundary;
            break;
        }
        case 0x21: {
            amINDX();
            opAND();
            opcycles += 6;
            break;
        }
        case 0x31: {
            amINDY();
            opAND();
            opcycles += 5 + crossedPageBoundary;
            break;
        }

        // ASL calls (arithmetic shift left)
        case 0x0A: {
            amACC();
            opASL();
            opcycles += 2;
            break;
        }
        case 0x06: {
            amZP();
            opASL();
            opcycles += 5;
            break;
        }
        case 0x16: {
            amZPX();
            opASL();
            opcycles += 6;
            break;
        }
        case 0x0E: {
            amABS();
            opASL();
            opcycles += 6;
            break;
        }
        case 0x1E: {
            amABSX();
            opASL();
            opcycles += 7;
            break;
        }

        // BIT calls
        case 0x24: {
            amZP();
            opBIT();
            opcycles += 3;
            break;
        }
        case 0x2C: {
            amABS();
            opBIT();
            opcycles += 4;
            break;
        }

        // Branch calls
        case 0x10: {
            amREL();
            opBPL();
            opcycles += 2 + isBranchTaken + crossedPageBoundary;
            break;
        }
        case 0x30: {
            amREL();
            opBMI();
            opcycles += 2 + isBranchTaken + crossedPageBoundary;
            break;
        }
        case 0x50: {
            amREL();
            opBVC();
            opcycles += 2 + isBranchTaken + crossedPageBoundary;
            break;
        }
        case 0x70: {
            amREL();
            opBVS();
            opcycles += 2 + isBranchTaken + crossedPageBoundary;
            break;
        }
        case 0x90: {
            amREL();
            opBCC();
            opcycles += 2 + isBranchTaken + crossedPageBoundary;
            break;
        }
        case 0xB0: {
            amREL();
            opBCS();
            opcycles += 2 + isBranchTaken + crossedPageBoundary;
            break;
        }
        case 0xD0: {
            amREL();
            opBNE();
            opcycles += 2 + isBranchTaken + crossedPageBoundary;
            break;
        }
        case 0xF0: {
            amREL();
            opBEQ();
            opcycles += 2 + isBranchTaken + crossedPageBoundary;
            break;
        }

        // BRK calls (break)
        case 0x00: {
            amIMP();
            opBRK();
            opcycles += 7;
            break;
        }

        // CMP calls (compare accumulator)
        case 0xC9: {
            amIMM();
            opCMP();
            opcycles += 2;
            break;
        }
        case 0xC5: {
            amZP();
            opCMP();
            opcycles += 3;
            break;
        }
        case 0xD5: {
            amZPX();
            opCMP();
            opcycles += 4;
            break;
        }
        case 0xCD: {
            amABS();
            opCMP();
            opcycles += 4;
            break;
        }
        case 0xDD: {
            amABSX();
            opCMP();
            opcycles += 4 + crossedPageBoundary;
            break;
        }
        case 0xD9: {
            amABSY();
            opCMP();
            opcycles += 4 + crossedPageBoundary;
            break;
        }
        case 0xC1: {
            amINDX();
            opCMP();
            opcycles += 6;
            break;
        }
        case 0xD1: {
            amINDY();
            opCMP();
            opcycles += 5 + crossedPageBoundary;
            break;
        }

        // CPX calls (compare X register)
        case 0xE0: {
            amIMM();
            opCPX();
            opcycles += 2;
            break;
        }
        case 0xE4: {
            amZP();
            opCPX();
            opcycles += 3;
            break;
        }
        case 0xEC: {
            amABS();
            opCPX();
            opcycles += 4;
            break;
        }

        // CPY calls (compare Y register)
        case 0xC0: {
            amIMM();
            opCPY();
            opcycles += 2;
            break;
        }
        case 0xC4: {
            amZP();
            opCPY();
            opcycles += 3;
            break;
        }
        case 0xCC: {
            amABS();
            opCPY();
            opcycles += 4;
            break;
        }

        // DEC calls (decrement)
        case 0xC6: {
            amZP();
            opDEC();
            opcycles += 5;
            break;
        }
        case 0xD6: {
            amZPX();
            opDEC();
            opcycles += 6;
            break;
        }
        case 0xCE: {
            amABS();
            opDEC();
            opcycles += 6;
            break;
        }
        case 0xDE: {
            amABSX();
            opDEC();
            opcycles += 7;
            break;
        }

        // EOR calls (bitwise exclusive or)
        case 0x49: {
            amIMM();
            opEOR();
            opcycles += 2;
            break;
        }
        case 0x45: {
            amZP();
            opEOR();
            opcycles += 3;
            break;
        }
        case 0x55: {
            amZPX();
            opEOR();
            opcycles += 4;
            break;
        }
        case 0x4D: {
            amABS();
            opEOR();
            opcycles += 4;
            break;
        }
        case 0x5D: {
            amABSX();
            opEOR();
            opcycles += 4 + crossedPageBoundary;
            break;
        }
        case 0x59: {
            amABSY();
            opEOR();
            opcycles += 4 + crossedPageBoundary;
            break;
        }
        case 0x41: {
            amINDX();
            opEOR();
            opcycles += 6;
            break;
        }
        case 0x51: {
            amINDY();
            opEOR();
            opcycles += 5 + crossedPageBoundary;
            break;
        }

        // Flag instructions call (processor status)
        case 0x18: {
            amIMP();
            opCLC();
            opcycles += 2;
            break;
        }
        case 0x38: {
            amIMP();
            opSEC();
            opcycles += 2;
            break;
        }
        case 0x58: {
            amIMP();
            opCLI();
            opcycles += 2;
            break;
        }
        case 0x78: {
            amIMP();
            opSEI();
            opcycles += 2;
            break;
        }
        case 0xB8: {
            amIMP();
            opCLV();
            opcycles += 2;
            break;
        }
        case 0xD8: {
            amIMP();
            opCLD();
            opcycles += 2;
            break;
        }
        case 0xF8: {
            amIMP();
            opSED();
            opcycles += 2;
            break;
        }

        // INC calls (increment memory)
        case 0xE6: {
            amZP();
            opINC();
            opcycles += 5;
            break;
        }
        case 0xF6: {
            amZPX();
            opINC();
            opcycles += 6;
            break;
        }
        case 0xEE: {
            amABS();
            opINC();
            opcycles += 6;
            break;
        }
        case 0xFE: {
            amABSX();
            opINC();
            opcycles += 7;
            break;
        }

        // JMP calls (jump)
        case 0x4C: {
            amABS();
            opJMP();
            opcycles += 3;
            break;
        }
        case 0x6C: {
            amIND();
            opJMP();
            opcycles += 5;
            break;
        }

        // JSR calls (jump to subroutine)
        case 0x20: {
            amABS();
            opJSR();
            opcycles += 6;
            break;
        }

        // LDA calls (load accumulator)
        case 0xA9: {
            amIMM();
            opLDA();
            opcycles += 2;
            break;
        }
        case 0xA5: {
            amZP();
            opLDA();
            opcycles += 3;
            break;
        }
        case 0xB5: {
            amZPX();
            opLDA();
            opcycles += 4;
            break;
        }
        case 0xAD: {
            amABS();
            opLDA();
            opcycles += 4;
            break;
        }
        case 0xBD: {
            amABSX();
            opLDA();
            opcycles += 4 + crossedPageBoundary;
            break;
        }
        case 0xB9: {
            amABSY();
            opLDA();
            opcycles += 4 + crossedPageBoundary;
            break;
        }
        case 0xA1: {
            amINDX();
            opLDA();
            opcycles += 6;
            break;
        }
        case 0xB1: {
            amINDY();
            opLDA();
            opcycles += 5 + crossedPageBoundary;
            break;
        }

        // LDX calls (load X register)
        case 0xA2: {
            amIMM();
            opLDX();
            opcycles += 2;
            break;
        }
        case 0xA6: {
            amZP();
            opLDX();
            opcycles += 3;
            break;
        }
        case 0xB6: {
            amZPY();
            opLDX();
            opcycles += 4;
            break;
        }
        case 0xAE: {
            amABS();
            opLDX();
            opcycles += 4;
            break;
        }
        case 0xBE: {
            amABSY();
            opLDX();
            opcycles += 4 + crossedPageBoundary;
            break;
        }

        // LDY (load Y register)
        case 0xA0: {
            amIMM();
            opLDY();
            opcycles += 2;
            break;
        }
        case 0xA4: {
            amZP();
            opLDY();
            opcycles += 3;
            break;
        }
        case 0xB4: {
            amZPX();
            opLDY();
            opcycles += 4;
            break;
        }
        case 0xAC: {
            amABS();
            opLDY();
            opcycles += 4;
            break;
        }
        case 0xBC: {
            amABSX();
            opLDY();
            opcycles += 4 + crossedPageBoundary;
            break;
        }

        // LSR calls (logical shift right)
        case 0x4A: {
            amACC();
            opLSR();
            opcycles += 2;
            break;
        }
        case 0x46: {
            amZP();
            opLSR();
            opcycles += 5;
            break;
        }
        case 0x56: {
            amZPX();
            opLSR();
            opcycles += 6;
            break;
        }
        case 0x4E: {
            amABS();
            opLSR();
            opcycles += 6;
            break;
        }
        case 0x5E: {
            amABSX();
            opLSR();
            opcycles += 7;
            break;
        }

        // NOP calls (no operation)
        case 0xEA: {
            amIMP();
            opNOP();
            opcycles += 2;
            break;
        }

        // ORA calls (bitwise or with accumulator)
        case 0x09: {
            amIMM();
            opORA();
            opcycles += 2;
            break;
        }
        case 0x05: {
            amZP();
            opORA();
            opcycles += 3;
            break;
        }
        case 0x15: {
            amZPX();
            opORA();
            opcycles += 4;
            break;
        }
        case 0x0D: {
            amABS();
            opORA();
            opcycles += 4;
            break;
        }
        case 0x1D: {
            amABSX();
            opORA();
            opcycles += 4 + crossedPageBoundary;
            break;
        }
        case 0x19: {
            amABSY();
            opORA();
            opcycles +=  + crossedPageBoundary;
            break;
        }
        case 0x01: {
            amINDX();
            opORA();
            opcycles += 6;
            break;
        }
        case 0x11: {
            amINDY();
            opORA();
            opcycles += 5 + crossedPageBoundary;
            break;
        }

        // Register instructions calls
        case 0xAA: {
            amIMP();
            opTAX();
            opcycles += 2;
            break;
        }
        case 0x8A: {
            amIMP();
            opTXA();
            opcycles += 2;
            break;
        }
        case 0xCA: {
            amIMP();
            opDEX();
            opcycles += 2;
            break;
        }
        case 0xE8: {
            amIMP();
            opINX();
            opcycles += 2;
            break;
        }
        case 0xA8: {
            amIMP();
            opTAY();
            opcycles += 2;
            break;
        }
        case 0x98: {
            amIMP();
            opTYA();
            opcycles += 2;
            break;
        }
        case 0x88: {
            amIMP();
            opDEY();
            opcycles += 2;
            break;
        }
        case 0xC8: {
            amIMP();
            opINY();
            opcycles += 2;
            break;
        }

        // ROL calls (rotate left)
        case 0x2A: {
            amACC();
            opROL();
            opcycles += 2;
            break;
        }
        case 0x26: {
            amZP();
            opROL();
            opcycles += 5;
            break;
        }
        case 0x36: {
            amZPX();
            opROL();
            opcycles += 6;
            break;
        }
        case 0x2E: {
            amABS();
            opROL();
            opcycles += 6;
            break;
        }
        case 0x3E: {
            amABSX();
            opROL();
            opcycles += 7;
            break;
        }

        // ROR calls (rotate right)
        case 0x6A: {
            amACC();
            opROR();
            opcycles += 2;
            break;
        }
        case 0x66: {
            amZP();
            opROR();
            opcycles += 5;
            break;
        }
        case 0x76: {
            amZPX();
            opROR();
            opcycles += 6;
            break;
        }
        case 0x6E: {
            amABS();
            opROR();
            opcycles += 6;
            break;
        }
        case 0x7E: {
            amABSX();
            opROR();
            opcycles += 7;
            break;
        }

        // RTI calls (return from interrupt)
        case 0x40: {
            amIMP();
            opRTI();
            opcycles += 6;
            break;
        }

        // RTS calls (return from subroutine)
        case 0x60: {
            amIMP();
            opRTS();
            opcycles += 6;
            break;
        }

        // SBC calls (subtract with carry)
        case 0xE9: {
            amIMM();
            opSBC();
            opcycles += 2;
            break;
        }
        case 0xE5: {
            amZP();
            opSBC();
            opcycles += 3;
            break;
        }
        case 0xF5: {
            amZPX();
            opSBC();
            opcycles += 4;
            break;
        }
        case 0xED: {
            amABS();
            opSBC();
            opcycles += 4;
            break;
        }
        case 0xFD: {
            amABSX();
            opSBC();
            opcycles += 4 + crossedPageBoundary;
            break;
        }
        case 0xF9: {
            amABSY();
            opSBC();
            opcycles += 4 + crossedPageBoundary;
            break;
        }
        case 0xE1: {
            amINDX();
            opSBC();
            opcycles += 6;
            break;
        }
        case 0xF1: {
            amINDY();
            opSBC();
            opcycles += 5 + crossedPageBoundary;
            break;
        }

        // STA calls (store accumulator)
        case 0x85: {
            amZP();
            opSTA();
            opcycles += 3;
            break;
        }
        case 0x95: {
            amZPX();
            opSTA();
            opcycles += 4;
            break;
        }
        case 0x8D: {
            amABS();
            opSTA();
            opcycles += 4;
            break;
        }
        case 0x9D: {
            amABSX();
            opSTA();
            opcycles += 5;
            break;
        }
        case 0x99: {
            amABSY();
            opSTA();
            opcycles += 5;
            break;
        }
        case 0x81: {
            amINDX();
            opSTA();
            opcycles += 6;
            break;
        }
        case 0x91: {
            amINDY();
            opSTA();
            opcycles += 6;
            break;
        }

        // Stack instructions calls
        case 0x9A: {
            amIMP();
            opTXS();
            opcycles += 2;
            break;
        }
        case 0xBA: {
            amIMP();
            opTSX();
            opcycles += 2;
            break;
        }
        case 0x48: {
            amIMP();
            opPHA();
            opcycles += 3;
            break;
        }
        case 0x68: {
            amIMP();
            opPLA();
            opcycles += 4;
            break;
        }
        case 0x08: {
            amIMP();
            opPHP();
            opcycles += 3;
            break;
        }
        case 0x28: {
            amIMP();
            opPLP();
            opcycles += 4;
            break;
        }

        // STX calls (store X register)
        case 0x86: {
            amZP();
            opSTX();
            opcycles += 3;
            break;
        }
        case 0x96: {
            amZPY();
            opSTX();
            opcycles += 4;
            break;
        }
        case 0x8E: {
            amABS();
            opSTX();
            opcycles += 4;
            break;
        }

        // STY calls (store Y register)
        case 0x84: {
            amZP();
            opSTY();
            opcycles += 3;
            break;
        }
        case 0x94: {
            amZPX();
            opSTY();
            opcycles += 4;
            break;
        }
        case 0x8C: {
            amABS();
            opSTY();
            opcycles += 4;
            break;
        }

        default: handleInstructionOperation();
    }
}