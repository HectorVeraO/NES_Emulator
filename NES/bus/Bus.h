#pragma once

#include <cstdint>
#include <array>
#include "Cartridge.h"

class Bus {
public:
    Bus();
    virtual ~Bus();

    virtual void reset() = 0;
    virtual void clock() = 0;
    virtual void loadCartridge(std::shared_ptr<Cartridge> const& cartridge) = 0;

    [[nodiscard]] virtual uint8_t readCPUMemory(uint16_t address) = 0;
    virtual void writeCPUMemory(uint16_t address, uint8_t value) = 0;
};