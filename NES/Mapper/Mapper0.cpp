//
// Created by junds on 04-09-2021.
//

#include "Mapper0.h"

Mapper0::Mapper0(uint8_t sPRGBankCount, uint8_t sCHRBankCount) {
    prgBankCount = sPRGBankCount;
    chrBankCount = sCHRBankCount;
}

Mapper0::~Mapper0() = default;

uint16_t Mapper0::mapToPRG(uint16_t address) const {
    return prgBankCount == 1 ? address % 0x4000 : address;
}

void Mapper0::handlePRGWrite(uint16_t address, uint8_t value) {

}