#pragma once

#include <cstdint>
#include <array>
#include "../Cartridge/Cartridge.h"

class MOS6502;

class Bus {
public:
    Bus();

    ~Bus();

    void connectCartridge(const std::string& cartridgePath);

    uint8_t readMemory(uint16_t address);

    void writeMemory(uint16_t address, uint8_t value);

private:
    Cartridge *cartridge;

    std::array<uint8_t, 0x0800> ram{};
    std::array<uint8_t, 0x0028> ioRegisters{};
    std::array<uint8_t, 0x1FE0> expansionROM{};
    std::array<uint8_t, 0x2000> sram{};
};