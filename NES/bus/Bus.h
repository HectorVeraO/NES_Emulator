#pragma once

#include <cstdint>
#include <array>

class MOS6502;

class Bus {
public:
    Bus();

    ~Bus();

    uint8_t readRAM(uint16_t address);

    void writeRAM(uint16_t address, uint8_t value);

private:
    std::array<uint8_t, 0xFFFF> RAM;
};