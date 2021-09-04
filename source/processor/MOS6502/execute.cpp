//
// Created by junds on 04-09-2021.
//

#include "MOS6502.h"

void MOS6502::execute() {
    opcode = readMemory(PC);
    PC++;

    switch (opcode) {
        // ADC calls (add with carry)
        case 0x69: {
            amIMM();
            opADC();
            break;
        }
        case 0x65: {
            amZP();
            opADC();
            break;
        }
        case 0x75: {
            amZPX();
            opADC();
            break;
        }
        case 0x6D: {
            amABS();
            opADC();
            break;
        }
        case 0x7D: {
            amABSX();
            opADC();
            break;
        }
        case 0x79: {
            amABSY();
            opADC();
            break;
        }
        case 0x61: {
            amINDX();
            opADC();
            break;
        }
        case 0x71: {
            amINDY();
            opADC();
            break;
        }

            // AND calls (bitwise and)
        case 0x29: {
            amIMM();
            opAND();
            break;
        }
        case 0x25: {
            amZP();
            opAND();
            break;
        }
        case 0x35: {
            amZPX();
            opAND();
            break;
        }
        case 0x2D: {
            amABS();
            opAND();
            break;
        }
        case 0x3D: {
            amABSX();
            opAND();
            break;
        }
        case 0x39: {
            amABSY();
            opAND();
            break;
        }
        case 0x21: {
            amINDX();
            opAND();
            break;
        }
        case 0x31: {
            amINDY();
            opAND();
            break;
        }

            // ASL calls (arithmetic shift left)
        case 0x0A: {
            amACC();
            opASL();
            break;
        }
        case 0x06: {
            amZP();
            opASL();
            break;
        }
        case 0x16: {
            amZPX();
            opASL();
            break;
        }
        case 0x0E: {
            amABS();
            opASL();
            break;
        }
        case 0x1E: {
            amABSX();
            opASL();
            break;
        }

            // BIT calls
        case 0x24: {
            amZP();
            opBIT();
            break;
        }
        case 0x2C: {
            amABS();
            opBIT();
            break;
        }

            // Branch calls
        case 0x10: {
            amREL();
            opBPL();
            break;
        }
        case 0x30: {
            amREL();
            opBMI();
            break;
        }
        case 0x50: {
            amREL();
            opBVC();
            break;
        }
        case 0x70: {
            amREL();
            opBVS();
            break;
        }
        case 0x90: {
            amREL();
            opBCC();
            break;
        }
        case 0xB0: {
            amREL();
            opBCS();
            break;
        }
        case 0xD0: {
            amREL();
            opBNE();
            break;
        }
        case 0xF0: {
            amREL();
            opBEQ();
            break;
        }

            // BRK calls (break)
        case 0x00: {
            amIMP();
            opBRK();
            break;
        }

            // CMP calls (compare accumulator)
        case 0xC9: {
            amIMM();
            opCMP();
            break;
        }
        case 0xC5: {
            amZP();
            opCMP();
            break;
        }
        case 0xD5: {
            amZPX();
            opCMP();
            break;
        }
        case 0xCD: {
            amABS();
            opCMP();
            break;
        }
        case 0xDD: {
            amABSX();
            opCMP();
            break;
        }
        case 0xD9: {
            amABSY();
            opCMP();
            break;
        }
        case 0xC1: {
            amINDX();
            opCMP();
            break;
        }
        case 0xD1: {
            amINDY();
            opCMP();
            break;
        }

            // CPX calls (compare X register)
        case 0xE0: {
            amIMM();
            opCPX();
            break;
        }
        case 0xE4: {
            amZP();
            opCPX();
            break;
        }
        case 0xEC: {
            amABS();
            opCPX();
            break;
        }

            // CPY calls (compare Y register)
        case 0xC0: {
            amIMM();
            opCPY();
            break;
        }
        case 0xC4: {
            amZP();
            opCPY();
            break;
        }
        case 0xCC: {
            amABS();
            opCPY();
            break;
        }

            // DEC calls (decrement)
        case 0xC6: {
            amZP();
            opDEC();
            break;
        }
        case 0xD6: {
            amZPX();
            opDEC();
            break;
        }
        case 0xCE: {
            amABS();
            opDEC();
            break;
        }
        case 0xDE: {
            amABSX();
            opDEC();
            break;
        }

            // EOR calls (bitwise exclusive or)
        case 0x49: {
            amIMM();
            opEOR();
            break;
        }
        case 0x45: {
            amZP();
            opEOR();
            break;
        }
        case 0x55: {
            amZPX();
            opEOR();
            break;
        }
        case 0x4D: {
            amABS();
            opEOR();
            break;
        }
        case 0x5D: {
            amABSX();
            opEOR();
            break;
        }
        case 0x59: {
            amABSY();
            opEOR();
            break;
        }
        case 0x41: {
            amINDX();
            opEOR();
            break;
        }
        case 0x51: {
            amINDY();
            opEOR();
            break;
        }

            // Flag instructions call (processor status)
        case 0x18: {
            amIMP();
            opCLC();
            break;
        }
        case 0x38: {
            amIMP();
            opSEC();
            break;
        }
        case 0x58: {
            amIMP();
            opCLI();
            break;
        }
        case 0x78: {
            amIMP();
            opSEI();
            break;
        }
        case 0xB8: {
            amIMP();
            opCLV();
            break;
        }
        case 0xD8: {
            amIMP();
            opCLD();
            break;
        }
        case 0xF8: {
            amIMP();
            opSED();
            break;
        }

            // INC calls (increment memory)
        case 0xE6: {
            amZP();
            opINC();
            break;
        }
        case 0xF6: {
            amZPX();
            opINC();
            break;
        }
        case 0xEE: {
            amABS();
            opINC();
            break;
        }
        case 0xFE: {
            amABSX();
            opINC();
            break;
        }

            // JMP calls (jump)
        case 0x4C: {
            amABS();
            opJMP();
            break;
        }
        case 0x6C: {
            amIND();
            opJMP();
            break;
        }

            // JSR calls (jump to subroutine)
        case 0x20: {
            amABS();
            opJSR();
            break;
        }

            // LDA calls (load accumulator)
        case 0xA9: {
            amIMM();
            opLDA();
            break;
        }
        case 0xA5: {
            amZP();
            opLDA();
            break;
        }
        case 0xB5: {
            amZPX();
            opLDA();
            break;
        }
        case 0xAD: {
            amABS();
            opLDA();
            break;
        }
        case 0xBD: {
            amABSX();
            opLDA();
            break;
        }
        case 0xB9: {
            amABSY();
            opLDA();
            break;
        }
        case 0xA1: {
            amINDX();
            opLDA();
            break;
        }
        case 0xB1: {
            amINDY();
            opLDA();
            break;
        }

            // LDX calls (load X register)
        case 0xA2: {
            amIMM();
            opLDX();
            break;
        }
        case 0xA6: {
            amZP();
            opLDX();
            break;
        }
        case 0xB6: {
            amZPY();
            opLDX();
            break;
        }
        case 0xAE: {
            amABS();
            opLDX();
            break;
        }
        case 0xBE: {
            amABSY();
            opLDX();
            break;
        }

            // LDY (load Y register)
        case 0xA0: {
            amIMM();
            opLDY();
            break;
        }
        case 0xA4: {
            amZP();
            opLDY();
            break;
        }
        case 0xB4: {
            amZPX();
            opLDY();
            break;
        }
        case 0xAC: {
            amABS();
            opLDY();
            break;
        }
        case 0xBC: {
            amABSX();
            opLDY();
            break;
        }

            // LSR calls (logical shift right)
        case 0x4A: {
            amACC();
            opLSR();
            break;
        }
        case 0x46: {
            amZP();
            opLSR();
            break;
        }
        case 0x56: {
            amZPX();
            opLSR();
            break;
        }case 0x4E: {
            amABS();
            opLSR();
            break;
        }
        case 0x5E: {
            amABSX();
            opLSR();
            break;
        }

            // NOP calls (no operation)
        case 0xEA: {
            amIMP();
            opNOP();
            break;
        }

            // ORA calls (bitwise or with accumulator)
        case 0x09: {
            amIMM();
            opORA();
            break;
        }
        case 0x05: {
            amZP();
            opORA();
            break;
        }
        case 0x15: {
            amZPX();
            opORA();
            break;
        }
        case 0x0D: {
            amABS();
            opORA();
            break;
        }
        case 0x1D: {
            amABSX();
            opORA();
            break;
        }
        case 0x19: {
            amABSY();
            opORA();
            break;
        }
        case 0x01: {
            amINDX();
            opORA();
            break;
        }
        case 0x11: {
            amINDY();
            opORA();
            break;
        }

            // Register instructions calls
        case 0xAA: {
            amIMP();
            opTAX();
            break;
        }
        case 0x8A: {
            amIMP();
            opTXA();
            break;
        }
        case 0xCA: {
            amIMP();
            opDEX();
            break;
        }
        case 0xE8: {
            amIMP();
            opINX();
            break;
        }
        case 0xA8: {
            amIMP();
            opTAY();
            break;
        }
        case 0x98: {
            amIMP();
            opTYA();
            break;
        }
        case 0x88: {
            amIMP();
            opDEY();
            break;
        }
        case 0xC8: {
            amIMP();
            opINY();
            break;
        }

            // ROL calls (rotate left)
        case 0x2A: {
            amACC();
            opROL();
            break;
        }
        case 0x26: {
            amZP();
            opROL();
            break;
        }
        case 0x36: {
            amZPX();
            opROL();
            break;
        }
        case 0x2E: {
            amABS();
            opROL();
            break;
        }
        case 0x3E: {
            amABSX();
            opROL();
            break;
        }

            // ROR calls (rotate right)
        case 0x6A: {
            amACC();
            opROR();
            break;
        }
        case 0x66: {
            amZP();
            opROR();
            break;
        }
        case 0x76: {
            amZPX();
            opROR();
            break;
        }
        case 0x6E: {
            amABS();
            opROR();
            break;
        }
        case 0x7E: {
            amABSX();
            opROR();
            break;
        }

            // RTI calls (return from interrupt)
        case 0x40: {
            amIMP();
            opRTI();
            break;
        }

            // RTS calls (return from subroutine)
        case 0x60: {
            amIMP();
            opRTS();
            break;
        }

            // SBC calls (subtract with carry)
        case 0xE9: {
            amIMM();
            opSBC();
            break;
        }
        case 0xE5: {
            amZP();
            opSBC();
            break;
        }
        case 0xF5: {
            amZPX();
            opSBC();
            break;
        }
        case 0xED: {
            amABS();
            opSBC();
            break;
        }
        case 0xFD: {
            amABSX();
            opSBC();
            break;
        }
        case 0xF9: {
            amABSY();
            opSBC();
            break;
        }
        case 0xE1: {
            amINDX();
            opSBC();
            break;
        }
        case 0xF1: {
            amINDY();
            opSBC();
            break;
        }

            // STA calls (store accumulator)
        case 0x85: {
            amZP();
            opSTA();
            break;
        }
        case 0x95: {
            amZPX();
            opSTA();
            break;
        }
        case 0x8D: {
            amABS();
            opSTA();
            break;
        }
        case 0x9D: {
            amABSX();
            opSTA();
            break;
        }
        case 0x99: {
            amABSY();
            opSTA();
            break;
        }
        case 0x81: {
            amINDX();
            opSTA();
            break;
        }
        case 0x91: {
            amINDY();
            opSTA();
            break;
        }

            // Stack instructions calls
        case 0x9A: {
            amIMP();
            opTXS();
            break;
        }
        case 0xBA: {
            amIMP();
            opTSX();
            break;
        }
        case 0x48: {
            amIMP();
            opPHA();
            break;
        }
        case 0x68: {
            amIMP();
            opPLA();
            break;
        }
        case 0x08: {
            amIMP();
            opPHP();
            break;
        }
        case 0x28: {
            amIMP();
            opPLP();
            break;
        }

            // STX calls (store X register)
        case 0x86: {
            amZP();
            opSTX();
            break;
        }
        case 0x96: {
            amZPY();
            opSTX();
            break;
        }
        case 0x8E: {
            amABS();
            opSTX();
            break;
        }

            // STY calls (store Y register)
        case 0x84: {
            amZP();
            opSTY();
            break;
        }
        case 0x94: {
            amZPX();
            opSTY();
            break;
        }
        case 0x8C: {
            amABS();
            opSTY();
            break;
        }

        default: opUnimplementedInstruction();
    }
}