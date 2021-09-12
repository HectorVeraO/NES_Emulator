//
// Created by junds on 10-09-2021.
//

#include "RawBus.h"

RawBus::RawBus(std::string const& mos6502ROMPath) {
    std::ifstream romifs(mos6502ROMPath, std::ios::ate);
    if (romifs.is_open()) {
        std::streampos size = romifs.tellg();
        memory.resize(size);
        romifs.seekg(0, std::ios::beg);
        romifs.read(reinterpret_cast<char*>(memory.data()), size);
        romifs.close();
    }
}

RawBus::~RawBus() = default;

uint8_t RawBus::readMemory(uint16_t address) const {
    return memory[address];
}

void RawBus::writeMemory(uint16_t address, uint8_t value) {
    memory[address] = value;
}

