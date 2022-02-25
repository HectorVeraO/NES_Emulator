//
// Created by junds on 09-09-2021.
//

#pragma once

#ifndef DISABLE_6502_LOGS
#define DISABLE_6502_LOGS 0
#endif

#include "MOS6502.h"
#include "NTSC2C02.h"
#include "Bus.h"

class NesBus : public Bus {
public:
    NesBus();
    ~NesBus() override;

    void powerUp();

    void clock() override;
    void reset() override;
    void loadCartridge(std::shared_ptr<Cartridge> const& cartridge) override;

    [[nodiscard]] uint8_t readCPUMemory(uint16_t address) override;
    void writeCPUMemory(uint16_t address, uint8_t value) override;
private:
    std::shared_ptr<spdlog::logger> logger = spdlog::get("mos6502");

    std::shared_ptr<Cartridge> cartridge;
    Canvas platform;
    MOS6502 cpu;
    NTSC2C02 ppu;

    std::vector<uint8_t> ram{};
    std::vector<uint8_t> ioRegisters{};
    std::vector<uint8_t> expansionROM{};
    std::vector<uint8_t> sram{};
    std::vector<uint8_t> patternTables{};
    std::vector<uint8_t> nameTables{};
    std::vector<uint8_t> palettes{};

    uint64_t totalCyclesPerformed{ 0 };
};



