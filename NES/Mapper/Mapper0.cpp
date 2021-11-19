//
// Created by junds on 04-09-2021.
//

#include "Mapper0.h"

Mapper0::Mapper0(uint8_t newPrgMemoryBankCount, uint8_t newChrMemoryBankCount) : Mapper(newPrgMemoryBankCount, newChrMemoryBankCount) {

}

Mapper0::~Mapper0() = default;

uint16_t Mapper0::mapToPRG(uint16_t address) const {
    return prgMemoryBankCount == 1 ? address % 0x4000 : address % 0x8000;
}

void Mapper0::handlePRGWrite(uint16_t address, uint8_t value) {

}

uint16_t Mapper0::mapToCHR(uint16_t address) const {
    return chrMemoryBankCount == 1 ? address % 0x2000 : address;
}

void Mapper0::handleCHRWrite(uint16_t address, uint8_t value) {

}

void Mapper0::reset() {

}
