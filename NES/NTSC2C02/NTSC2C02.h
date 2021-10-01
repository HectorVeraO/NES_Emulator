//
// Created by junds on 27-09-2021.
//

#pragma once

#include "Bus.h"

class NTSC2C02 {
public:
    void powerUp();
    void clock();
    void connectBus(Bus* newBus);

    // memory.cpp
    [[nodiscard]] uint8_t readMemory(uint16_t address) const;
    void writeMemory(uint16_t address, uint8_t value);
    [[nodiscard]] uint8_t readCPUMemory(uint16_t address) const;
    void writeCPUMemory(uint16_t address, uint8_t value);

private:
    Bus* bus;

    // Memory mapped registers
    uint8_t PPUCTRL;        // CPU RAM address $2000 - flags VPHB SINN
    uint8_t PPUMASK;        // CPU RAM address $2001 - flags BGRs bMmG
    uint8_t PPUSTATUS;      // CPU RAM address $2002 - flags VSO- ----
    uint8_t OAMADDR;        // CPU RAM address $2003 - flags aaaa aaaa
    uint8_t OAMDATA;        // CPU RAM address $2004 - flags dddd dddd
    uint8_t PPUSCROLL;      // CPU RAM address $2005 - flags xxxx xxxx
    uint8_t PPUADDR;        // CPU RAM address $2006 - flags aaaa aaaa
    uint8_t PPUDATA;        // CPU RAM address $2007 - flags dddd dddd
    uint8_t OAMDMA;         // CPU RAM address $4014 - flags aaaa aaaa
};



