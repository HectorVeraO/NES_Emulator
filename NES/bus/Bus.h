#pragma once

#include <cstdint>
#include <array>
#include "Cartridge.h"

class Bus {
public:
    Bus();
    virtual ~Bus();

    [[nodiscard]] virtual uint8_t readCPUMemory(uint16_t address) const = 0;
    virtual void writeCPUMemory(uint16_t address, uint8_t value) = 0;

    [[nodiscard]] virtual uint8_t readPPUMemory(uint16_t address) const = 0;
    virtual void writePPUMemory(uint16_t address, uint8_t value) = 0;
};