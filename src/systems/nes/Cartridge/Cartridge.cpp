//
// Created by junds on 06-09-2021.
//

#include "Cartridge.h"
#include "Mapper0.h"

Cartridge::Cartridge(const std::string& filePath) {
    std::ifstream romifs(filePath, std::ios::binary);
    if (romifs.is_open()) {
        romifs.read(reinterpret_cast<char*>(&header), sizeof header);

        if (header.format[0] != 0x1A)
            throw std::invalid_argument("Invalid file format, expected INES format.");

        if (header.romControl1 & 0x04)  // Skip trainer
            romifs.ignore(512);

        mirroring = header.romControl1 & 0x01 ? MirroringType::Vertical : MirroringType::Horizontal;
        hasBatteryBackedRAM = header.romControl1 & 0x02;
        useFourScreenMirroring = header.romControl1 & 0x08;
        mapperId = ((header.romControl2 & 0xF0) << 4) | header.romControl1 & 0xF0;
        switch (mapperId) {
            case 0x00: {
                mapper = std::make_shared<Mapper0>(header.prgRomBankCount, header.chrRomBankCount);
                break;
            }
            default: {
                std::cout << "Mapper" << mapperId << " is not implemented" << std::endl;
            }
        }

        if (header.prgRomBankCount == 0)
            header.prgRomBankCount = 1;
        if (header.chrRomBankCount == 0)
            header.chrRomBankCount = 1;

        prgMemoryBankCount = header.prgRomBankCount;
        uint32_t prgROMSize = prgMemoryBankCount * 0x4000;
        prgRom.resize(prgROMSize);
        romifs.read(reinterpret_cast<char*>(prgRom.data()), prgROMSize);

        chrMemoryBankCount = header.chrRomBankCount;
        uint32_t chrROMSize = chrMemoryBankCount * 0x2000;
        chrRom.resize(chrROMSize);
        romifs.read(reinterpret_cast<char*>(chrRom.data()), chrROMSize);

        isValidROM = true;
        romifs.close();
    }
}

Cartridge::~Cartridge() = default;

bool Cartridge::isValid() const {
    return isValidROM;
}

uint8_t Cartridge::readPRGMemory(uint16_t address) const {
    return prgRom[mapper->mapToPRG(address)];
}

void Cartridge::writePRGMemory(uint16_t address, uint8_t value) {
    mapper->handlePRGWrite(address, value);
    capturedPrgWrites.push(value);
}

uint8_t Cartridge::readCHRMemory(uint16_t address) const {
    return chrRom[mapper->mapToCHR(address)];
}

void Cartridge::writeCHRMemory(uint16_t address, uint8_t value) {
    mapper->handleCHRWrite(address, value);
}

Cartridge::MirroringType Cartridge::getMirroring() const {
    return mirroring;
}

void Cartridge::reset() {
    if (mapper != nullptr)
        mapper->reset();
}