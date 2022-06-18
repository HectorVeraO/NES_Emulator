//
// Created by junds on 10-09-2021.
//

#include "RawBus.h"

RawBus::RawBus(std::string const& mos6502ROMPath) {
    std::ifstream romifs(mos6502ROMPath, std::ios::ate);
    if (romifs.is_open()) {
        std::streampos size = romifs.tellg();
        cpuMemory.resize(size);
        romifs.seekg(0, std::ios::beg);
        romifs.read(reinterpret_cast<char*>(cpuMemory.data()), size);
        romifs.close();
    }
}

RawBus::RawBus(std::vector<uint8_t> rom) : cpuMemory(std::move(rom)) {

}

RawBus::~RawBus() = default;

uint8_t RawBus::readCPUMemory(uint16_t address) {
    return cpuMemory[address];
}

void RawBus::writeCPUMemory(uint16_t address, uint8_t value) {
    cpuMemory[address] = value;
}

void RawBus::reset() {

}

void RawBus::clock() {

}

void RawBus::loadCartridge(std::shared_ptr<Cartridge> const& cartridge) {

}
