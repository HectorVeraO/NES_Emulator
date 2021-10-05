//
// Created by junds on 09-09-2021.
//

#include "NesBus.h"

NesBus::NesBus() {
    cartridge = nullptr;
    for (auto& slot: ram)
        slot = 0x00;
}

NesBus::~NesBus() {
    delete cartridge;
}

void NesBus::connectCartridge(std::string const& cartridgePath) {
    cartridge = new Cartridge(cartridgePath);
}

uint8_t NesBus::readCPUMemory(uint16_t address) const {
    if (address < 0x2000)
        return ram[address % 0x0800];

    if (address < 0x4000)
        return ioRegisters[address % 0x0008];

    if (address < 0x4020)
        return ioRegisters[address % 0x0020];

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
        ioRegisters[address % 0x0008] = value;

    else if (address < 0x4020)
        ioRegisters[address % 0x0020] = value;

    else if (address < 0x6000)
        expansionROM[address % 0x1FE0] = value;

    else if (address < 0x8000)
        sram[address % 0x2000] = value;

    else
        cartridge->writePRGMemory(address, value);
}

// FIXME: Reading is extremely common make this faster, maybe all the heavy work should be made when writing
uint8_t NesBus::readPPUMemory(uint16_t address) const {
    if (address < 0x2000)
        return patternTables[address];

    if (address < 0x3F00)
        return nameTables[address % 0x0F00];

    if (address < 0x4000)
        return palettes[address % 0x00F0];

    return readPPUMemory(address - 0x4000);
}

void NesBus::writePPUMemory(uint16_t address, uint8_t value) {
    address %= 0x4000;

    if (address < 0x2000)
        patternTables[address] = value;

    else if (address < 0x3F00)
        nameTables[address % 0x0F00] = value;

    else
        palettes[address % 0x00F0] = value;
}
