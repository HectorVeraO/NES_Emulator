//
// Created by junds on 04-09-2021.
//

#pragma once


#include <cstdint>

class Mapper {
public:
    virtual ~Mapper() = 0;

    virtual uint16_t mapToPRG(uint16_t address) const = 0;

    virtual void handlePRGWrite(uint16_t address, uint8_t value) = 0;

protected:
    uint8_t prgBankCount{};
    uint8_t chrBankCount{};
};



