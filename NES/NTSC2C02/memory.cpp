//
// Created by junds on 29-09-2021.
//

#include "NTSC2C02.h"

uint8_t NTSC2C02::readPPUMemory(uint16_t address) const {
    return bus->readPPUMemory(address);
}

void NTSC2C02::writePPUMemory(uint16_t address, uint8_t value) {
    bus->writePPUMemory(address, value);
}

uint8_t NTSC2C02::readCPUMemory(uint16_t address) const {
    return bus->readCPUMemory(address);
}

void NTSC2C02::writeCPUMemory(uint16_t address, uint8_t value) {
    bus->writeCPUMemory(address, value);
}