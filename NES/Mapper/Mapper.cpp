//
// Created by junds on 04-09-2021.
//

#include "Mapper.h"

Mapper::Mapper(uint8_t newPrgMemoryBankCount, uint8_t newChrMemoryBankCount) {
    prgMemoryBankCount = newPrgMemoryBankCount;
    chrMemoryBankCount = newChrMemoryBankCount;
}

Mapper::~Mapper() = default;
