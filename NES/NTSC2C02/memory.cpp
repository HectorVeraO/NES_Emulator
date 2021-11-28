//
// Created by junds on 29-09-2021.
//

#include "NTSC2C02.h"

uint8_t NTSC2C02::readPPUMemory(uint16_t address) const {
    address &= 0x3FFF;
    uint8_t output = 0x00;

    if (address >= 0x0000 && address <= 0x1FFF) {
        //output = patternTables[(address & 0x1000) >> 12][address % 4096];
        output = cartridge->readCHRMemory(address);
    } else if (address >= 0x2000 && address <= 0x3EFF) {
        uint16_t taddr = address & 0x0FFF;
        switch (cartridge->getMirroring()) {
            case Cartridge::MirroringType::Vertical: {
                if ((taddr >= 0x0000 && taddr <= 0x03FF) || (taddr >= 0x0800 && taddr <= 0x0BFF))
                    output = nameTables[0][taddr & 0x03FF];
                else if ((taddr >= 0x0400 && taddr <= 0x07FF) || (taddr >= 0x0C00 && taddr <= 0x0FFF))
                    output = nameTables[1][taddr & 0x03FF];
                break;
            }
            case Cartridge::MirroringType::Horizontal: {
                if ((taddr >= 0x0000 && taddr <= 0x03FF) || (taddr >= 0x0400 && taddr <= 0x07FF))
                    output = nameTables[0][taddr & 0x03FF];
                else if ((taddr >= 0x0800 && taddr <= 0x0BFF) || (taddr >= 0x0C00 && taddr <= 0x0FFF))
                    output = nameTables[1][taddr & 0x03FF];
                break;
            }
            default: break;
        }
    } else if (address >= 0x3F00 && address <= 0x3FFF) {
        uint16_t taddr = address & 0x001F;
        switch (taddr) {
            case 0x0010: {
                taddr = 0x0000;
                break;
            }
            case 0x0014: {
                taddr = 0x0004;
                break;
            }
            case 0x0018: {
                taddr = 0x0008;
                break;
            }
            case 0x001C: {
                taddr = 0x000C;
                break;
            }
            default: break;
        }
        output = palettes[taddr] & (PPUMASK.G ? 0x30 : 0x3F);
    }

    return output;
}

void NTSC2C02::writePPUMemory(uint16_t address, uint8_t value) {
    address &= 0x3FFF;
    cartridge->writeCHRMemory(address, value);

    if (address >= 0x0000 && address <= 0x1FFF) {
        patternTables[(address & 0x1000) >> 12][address & 0x0FFF] = value;
    } else if (address >= 0x2000 && address <= 0x3EFF) {
        uint16_t taddr = address & 0x0FFF;
        switch (cartridge->getMirroring()) {
            case Cartridge::MirroringType::Vertical: {
                if ((taddr >= 0x0000 && taddr <= 0x03FF) || (taddr >= 0x0800 && taddr <= 0x0BFF))
                    nameTables[0][taddr & 0x03FF] = value;
                else if ((taddr >= 0x0400 && taddr <= 0x07FF) || (taddr >= 0x0C00 && taddr <= 0x0FFF))
                    nameTables[1][taddr & 0x03FF] = value;
                break;
            }
            case Cartridge::MirroringType::Horizontal: {
                if ((taddr >= 0x0000 && taddr <= 0x03FF) || (taddr >= 0x0400 && taddr <= 0x07FF))
                    nameTables[0][taddr & 0x03FF] = value;
                else if ((taddr >= 0x0800 && taddr <= 0x0BFF) || (taddr >= 0x0C00 && taddr <= 0x0FFF))
                    nameTables[1][taddr & 0x03FF] = value;
                break;
            }
            default: break;
        }
    } else if (address >= 0x3F00 && address <= 0x3FFF) {
        uint16_t taddr = address & 0x001F;
        switch (taddr) {
            case 0x0010: {
                taddr = 0x0000;
                break;
            }
            case 0x0014: {
                taddr = 0x0004;
                break;
            }
            case 0x0018: {
                taddr = 0x0008;
                break;
            }
            case 0x001C: {
                taddr = 0x000C;
                break;
            }
            default: break;
        }
        palettes[taddr] = value;
    }
}

uint8_t NTSC2C02::readIO(uint16_t address) {
    uint8_t output{ 0x00 };
    if (address >= 0x2000 && address < 0x4000) {
        switch (address % 8) {
            case 0: {
                // Access: Write
                //output = static_cast<uint8_t>(PPUCTRL);
                output = 0x00;
                break;
            }
            case 1: {
                // Access: Write
                //output = static_cast<uint8_t>(PPUMASK);
                output = 0x00;
                break;
            }
            case 2: {
                auto status = static_cast<uint8_t>(PPUSTATUS);
                output = (status & 0xE0) | (dataBuffer & 0x1F);
                PPUSTATUS.V = false;
                loopy.w.reset();
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
                output = dataBuffer;
                auto v = static_cast<uint16_t>(loopy.v);
                dataBuffer = readPPUMemory(v);

                if (v >= 0x3F00) {
                    output = dataBuffer;
                }

                loopy.v = v + (PPUCTRL.I ? 32 : 1);
                break;
            }
        }
    }

    return output;
}

void NTSC2C02::writeIO(uint16_t address, uint8_t value) {
    if (address >= 0x2000 && address < 0x4000) {
        switch (address % 8) {
            case 0: {
                PPUCTRL = value;
                auto NN = PPUCTRL.NN.to_ulong();
                loopy.t.nametableX = (NN >> 0) & 0x01;
                loopy.t.nametableY = (NN >> 1) & 0x01;
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
                PPUSCROLL = value;
                if (loopy.w.none()) {
                    loopy.t.coarseX = value >> 3;
                    loopy.fineX = value & 0x07;
                    loopy.w.set();
                } else {
                    loopy.t.coarseY = value >> 3;
                    loopy.t.fineY = value & 0x07;
                    loopy.w.reset();
                }
                break;
            }
            case 6: {
                PPUADDR = value;
                auto t = static_cast<uint16_t>(loopy.t);
                if (loopy.w.none()) {
                    loopy.t = ((value & 0x3F) << 8) | (t & 0x00FF);
                    loopy.w.set();
                } else {
                    loopy.t = (t & 0xFF00) | value;
                    loopy.v = loopy.t;
                    loopy.w.reset();
                }
                break;
            }
            case 7: {
                auto v = static_cast<uint16_t>(loopy.v);
                writePPUMemory(v, value);
                PPUDATA = value;
                loopy.v = v + (PPUCTRL.I ? 32 : 1);
                break;
            }
        }
    }
}