//
// Created by junds on 27-09-2021.
//

#include "NTSC2C02.h"

void NTSC2C02::powerUp() {
    PPUCTRL = 0x00;
    PPUMASK = 0x00;
    PPUSTATUS = 0x00;
    OAMADDR = 0x00;
    PPUSCROLL = 0x00;
    PPUADDR = 0x00;
    PPUDATA = 0x00;
}

void NTSC2C02::clock() {
}

void NTSC2C02::connectBus(Bus* newBus) {
    bus = newBus;
}