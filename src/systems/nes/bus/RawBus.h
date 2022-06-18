//
// Created by junds on 10-09-2021.
//

#pragma once

#include <utility>
#include "Bus.h"

class RawBus : public Bus {
public:
    explicit RawBus(std::string const& mos6502ROMPath);
    explicit RawBus(std::vector<uint8_t> rom);
    ~RawBus() override;

    void reset() override;
    void clock() override;
    void loadCartridge(std::shared_ptr<Cartridge> const& cartridge) override;

    [[nodiscard]] uint8_t readCPUMemory(uint16_t address) override;
    void writeCPUMemory(uint16_t address, uint8_t value) override;
private:
    std::vector<uint8_t> cpuMemory;
    std::vector<uint8_t> ppuMemory;
};



