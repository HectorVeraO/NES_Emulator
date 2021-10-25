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
    if (address >= 0x2000 && address < 0x4000) {
        switch (address % 7) {
            case 0: {

                break;
            }
            case 1: {

                break;
            }
            case 2: {

                break;
            }
            case 3: {

                break;
            }
            case 4: {

                break;
            }
            case 5: {

                break;
            }
            case 6: {

                break;
            }
            case 7: {

                break;
            }
        }
    }

    return bus->readCPUMemory(address);
}

void NTSC2C02::writeCPUMemory(uint16_t address, uint8_t value) {
    if (address >= 0x2000 && address < 0x4000) {
        switch (address % 7) {
            case 0: {
                PPUCTRL = value;
                break;
            }
            case 1: {
                PPUMASK = value;
                break;
            }
            case 2: {
                break;
            }
            case 3: {
                break;
            }
            case 4: {
                break;
            }
            case 5: {

                break;
            }
            case 6: {

                break;
            }
            case 7: {

                break;
            }
        }
    }

    bus->writeCPUMemory(address, value);
}