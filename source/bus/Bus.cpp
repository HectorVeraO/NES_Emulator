#include "Bus.h"

Bus::Bus() = default;

Bus::~Bus() = default;

uint8_t Bus::readRAM(uint16_t address) { return RAM[address]; }

void Bus::writeRAM(uint16_t address, uint8_t value) { RAM[address] = value; }