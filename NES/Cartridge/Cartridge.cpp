//
// Created by junds on 06-09-2021.
//

#include "Cartridge.h"
#include "Mapper0.h"

Cartridge::Cartridge(std::string sfilepath) : filepath(std::move(sfilepath)) {
    std::ifstream romifs(filepath, std::ios::binary);
    if (romifs.is_open()) {
        romifs.read(reinterpret_cast<char*>(&header), sizeof header);

        if (header.romControl1 & 0x04)  // Skip trainer
            romifs.ignore(512);

        mirroringType = header.romControl1 & 0x01 ? MirroringType::Vertical : MirroringType::Horizontal;
        hasBatteryBackedRAM = header.romControl1 & 0x02;
        useFourScreenMirroring = header.romControl1 & 0x08;
        uint8_t mapperNumber = header.romControl1 & 0xF0;
        switch (mapperNumber) {
            case 0x00: {
                mapper = new Mapper0(header.prgRomBankCount, header.chrRomBankCount);
                break;
            }
            default: {
                std::cout << "Mapper" << mapperNumber << " is not implemented" << std::endl;
            }
        }

        uint32_t prgROMSize = header.prgRomBankCount * 0x4000;
        prgROM.resize(prgROMSize);
        romifs.read(reinterpret_cast<char*>(prgROM.data()), prgROMSize);

        uint32_t chrROMSize = header.chrRomBankCount * 0x2000;
        chrROM.resize(chrROMSize);
        romifs.read(reinterpret_cast<char*>(chrROM.data()), chrROMSize);

        romifs.close();
    }
}

uint8_t Cartridge::readMemory(uint16_t address) const {
    return prgROM[mapper->mapToPRG(address)];
}

void Cartridge::writeMemory(uint16_t address, uint8_t value) {
    mapper->handlePRGWrite(address, value);
    capturedPRGWrites.push(value);
}

Cartridge::~Cartridge() {
    delete mapper;
}
