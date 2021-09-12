#pragma once

#include <cstdint>
#include <array>
#include "Cartridge.h"

class Bus {
public:
    Bus();

    virtual ~Bus();

    [[nodiscard]] virtual uint8_t readMemory(uint16_t address) const = 0;

    virtual void writeMemory(uint16_t address, uint8_t value) = 0;
};