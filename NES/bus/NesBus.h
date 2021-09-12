//
// Created by junds on 09-09-2021.
//

#pragma once


#include "../Cartridge/Cartridge.h"
#include "Bus.h"

class NesBus : public Bus {
public:
    NesBus();

    ~NesBus() override;

    void connectCartridge(std::string const& cartridgePath);

    uint8_t readMemory(uint16_t address) const override;

    void writeMemory(uint16_t address, uint8_t value) override;

private:
    Cartridge *cartridge;

    std::array<uint8_t, 0x0800> ram{};
    std::array<uint8_t, 0x0028> ioRegisters{};
    std::array<uint8_t, 0x1FE0> expansionROM{};
    std::array<uint8_t, 0x2000> sram{};
};



