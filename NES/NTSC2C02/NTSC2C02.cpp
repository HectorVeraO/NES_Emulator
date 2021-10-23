//
// Created by junds on 27-09-2021.
//

#include "NTSC2C02.h"

NTSC2C02::NTSC2C02() {
    bus = nullptr;
}

NTSC2C02::~NTSC2C02() = default;

void NTSC2C02::powerUp() {

}

void NTSC2C02::clock() {
    currentCycle++;
}

void NTSC2C02::connectBus(Bus* newBus) {
    bus = newBus;
}