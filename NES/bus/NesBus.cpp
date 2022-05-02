//
// Created by junds on 09-09-2021.
//

#include "NesBus.h"

NesBus::NesBus() : platform("WDNES", 256, 240), cpu(), ppu() {
    cartridge = nullptr;
    cpu.connectBus(this);
    ppu.connectPlatform(&platform);
    ram.resize(0x0800);
    ioRegisters.resize(0x0020);
    expansionROM.resize(0x1FE0);
    sram.resize(0x2000);
    patternTables.resize(0x2000);
    nameTables.resize(0x0F00);
    palettes.resize(0x0020);
}

NesBus::~NesBus() = default;

void NesBus::loadCartridge(std::shared_ptr<Cartridge> const& newCartridge) {
    cartridge = newCartridge;
    ppu.loadCartridge(newCartridge);
}

void NesBus::reset() {
    cartridge->reset();
    cpu.reset();
    ppu.reset();
    totalCyclesPerformed = 0;
}

void NesBus::powerUp() {
    cpu.addHook(MOS6502::Hook::AfterInstructionExecution, [&]() {
        auto state = cpu.getPreInstructionExecutionState();
        std::string firstOperand = state.operand.size >= MOS6502::InstructionOperand::Size::One ? fmt::format("{:0>2X}", state.operand.msb) : "  ";
        std::string secondOperand = state.operand.size >= MOS6502::InstructionOperand::Size::Two ? fmt::format("{:0>2X}", state.operand.lsb) : "  ";

#if !(DISABLE_6502_LOGS)
        logger->info("{:0>4X}  {:0>2X} {} {}  {} ${: <27X} A:{:0>2X} X:{:0>2X} Y:{:0>2X} P:{:0>2X} SP:{:0>2X} PPU:{: >3},{: >3} CYC:{}", state.PC, state.opcode, firstOperand, secondOperand, state.instructionName, 0x0000, state.A, state.X, state.Y, state.P, state.S, 0, 0, state.totalCycles);
#endif
    });

    bool quit = false;
    while (!quit) {
        clock();
        quit = platform.processInput();
    }
}

uint8_t NesBus::readCPUMemory(uint16_t address) {
    if (address < 0x2000)
        return ram[address % 0x0800];

    if (address < 0x4000)
        return ppu.readIO(address);

    if (address < 0x4020)
        return ioRegisters[address % 0x0020];   // TODO: Link controller state

    if (address < 0x6000)
        return expansionROM[address % 0x1FE0];

    if (address < 0x8000)
        return sram[address % 0x2000];

    return cartridge->readPRGMemory(address);
}

void NesBus::writeCPUMemory(uint16_t address, uint8_t value) {
    if (address < 0x2000)
        ram[address % 0x0800] = value;

    else if (address < 0x4000)
        ppu.writeIO(address, value);

    else if (address < 0x4020)
        ioRegisters[address % 0x0020] = value;  // TODO: Link controller state

    else if (address < 0x6000)
        expansionROM[address % 0x1FE0] = value;

    else if (address < 0x8000)
        sram[address % 0x2000] = value;

    else
        cartridge->writePRGMemory(address, value);
}

void NesBus::clock() {
    ppu.clock();
    if (totalCyclesPerformed % 3 == 0)
        cpu.clock();

    if (ppu.nmi) {
        ppu.nmi = false;
        cpu.nmi();
    }

    totalCyclesPerformed++;
}
