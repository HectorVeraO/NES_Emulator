//
// Created by junds on 04-09-2021.
//

#pragma once


#include <cstdint>
#include <array>
#include "Mapper.h"

class Mapper0 : public Mapper {
public:
    Mapper0(uint8_t newPrgMemoryBankCount, uint8_t newChrMemoryBankCount);
    ~Mapper0() override;

    [[nodiscard]] uint16_t mapToPRG(uint16_t address) const override;
    void handlePRGWrite(uint16_t address, uint8_t value) override;
    [[nodiscard]] uint16_t mapToCHR(uint16_t address) const override;
    void handleCHRWrite(uint16_t address, uint8_t value) override;

    void reset() override;
};



