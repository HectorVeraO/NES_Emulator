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

uint8_t NesBus::readMemory(uint16_t address) const {
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

    return cartridge->readMemory(address);
}

void NesBus::writeMemory(uint16_t address, uint8_t value) {
    if (address < 0x2000)
        ram[address % 0x0800] = value;

    if (address < 0x4000)
        ioRegisters[address % 0x0008] = value;

    if (address < 0x4020)
        ioRegisters[address % 0x0020] = value;

    if (address < 0x6000)
        expansionROM[address % 0x1FE0] = value;

    if (address < 0x8000)
        sram[address % 0x2000] = value;

    cartridge->writeMemory(address, value);
}
