#include "MOS6502.h"

MOS6502::MOS6502() {
    bus = nullptr;
    addressingMode = AddressingMode::None;
    PC = 0x0000;
    S = 0x00;
    P = 0x00;
    A = 0x00;
    X = 0x00;
    Y = 0x00;
    opcode = 0x00;
    irqHandler = &MOS6502::interruptNONE;
    totalCyclesPerformed = 0;
    setFlag(Flag::B, true);
}

MOS6502::MOS6502(uint16_t const& startingPC) {
    bus = nullptr;
    addressingMode = AddressingMode::None;
    PC = startingPC;
    S = 0x00;
    P = 0x00;
    A = 0x00;
    X = 0x00;
    Y = 0x00;
    opcode = 0x00;
    irqHandler = &MOS6502::interruptNONE;
    totalCyclesPerformed = 0;
    setFlag(Flag::B, true);
}

MOS6502::~MOS6502() = default;

void MOS6502::clock() {
    if (opcycles > 0) {
        opcycles--;
        totalCyclesPerformed++;
    } else {
        opcode = readMemory(PC);
        PC++;
        crossedPageBoundary = false;
        opcycles = 0;

        decodeOperation();
        (this->*irqHandler)();
    }
}

void MOS6502::step() {
   do {
       clock();
   } while (opcycles > 0);
}

void MOS6502::nmi() {
    interruptNMI();
}

void MOS6502::connectBus(Bus* newBus) {
    this->bus = newBus;
}

void MOS6502::reset() {
    interruptReset();
}

MOS6502::State MOS6502::getState() const {
    return { A, X, Y, S, P, PC, totalCyclesPerformed, opcode, opalias, { 0x00, 0x00, InstructionOperand::Size::Implicit }};
}

MOS6502::State MOS6502::getPreInstructionExecutionState() const {
    return preInstructionExecutionState;
}

bool MOS6502::requiresFetch() const {
    return fetchingInstructions.contains(opalias);
}

uint8_t MOS6502::readMemory(uint16_t address) const {
    return bus->readCPUMemory(address);
}

void MOS6502::writeMemory(uint16_t address, uint8_t value) const {
    bus->writeCPUMemory(address, value);
}

uint8_t MOS6502::pullStack() {
    uint16_t address = 0x0100 | ++S;
    uint8_t value = readMemory(address);
    return value;
}

void MOS6502::pushStack(uint8_t value) {
    uint16_t address = 0x0100 | S--;
    writeMemory(address, value);
}

uint8_t MOS6502::getFlag(uint8_t offset) const {
    return (P >> offset) & 1;
}

void MOS6502::setFlag(uint8_t offset, bool turnOn) {
    if (turnOn) {
        P |= (1 << offset);
    } else {
        P &= ~(1 << offset);
    }
}
