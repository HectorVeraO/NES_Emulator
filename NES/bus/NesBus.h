//
// Created by junds on 09-09-2021.
//

#pragma once


#include "MOS6502.h"
#include "NTSC2C02.h"
#include "Cartridge.h"
#include "Bus.h"

class NesBus : public Bus {
public:
    NesBus();
    ~NesBus() override;

    void connectCartridge(std::string const& cartridgePath);
    [[nodiscard]] uint8_t readCPUMemory(uint16_t address) const override;
    void writeCPUMemory(uint16_t address, uint8_t value) override;

    [[nodiscard]] uint8_t readPPUMemory(uint16_t address) const override;
    void writePPUMemory(uint16_t address, uint8_t value) override;

private:
    Cartridge* cartridge;
    MOS6502* cpu;
    NTSC2C02* ppu;

    std::array<uint8_t, 0x0800> ram{};
    std::array<uint8_t, 0x0028> ioRegisters{};
    std::array<uint8_t, 0x1FE0> expansionROM{};
    std::array<uint8_t, 0x2000> sram{};

    std::array<uint8_t, 0x2000> patternTables{};
    std::array<uint8_t, 0x0F00> nameTables{};
    std::array<uint8_t, 0x0020> palettes{};

    uint64_t totalCyclesPerformed{ 0 };

    void clock();
};



