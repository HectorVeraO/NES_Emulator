//
// Created by junds on 10-09-2021.
//

#pragma once

#include "Bus.h"

class RawBus : public Bus {
public:
    explicit RawBus(std::string const& mos6502ROMPath);
    explicit RawBus(std::vector<uint8_t> rom);
    ~RawBus() override;

    [[nodiscard]] uint8_t readMemory(uint16_t address) const override;
    void writeMemory(uint16_t address, uint8_t value) override;

private:
    std::vector<uint8_t> memory;
};



