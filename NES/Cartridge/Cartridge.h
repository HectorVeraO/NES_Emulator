//
// Created by junds on 06-09-2021.
//

#pragma once


#include <string>
#include <vector>
#include <stack>
#include <fstream>
#include <iostream>
#include "../Mapper/Mapper.h"

class Cartridge {
public:
    explicit Cartridge(std::string sfilepath);

    virtual ~Cartridge();

    [[nodiscard]] uint8_t readMemory(uint16_t address) const;

    void writeMemory(uint16_t address, uint8_t value);

private:
    struct INESHeader {
        char system[3];
        char format[1];
        uint8_t prgRomBankCount;
        uint8_t chrRomBankCount;
        uint8_t romControl1;
        uint8_t romControl2;
        uint8_t ramBankCount;   // 8 KB RAM banks, if 0 assume 1 page
        char unused[7];
    } header{};

    enum MirroringType {
        Vertical = 1,
        Horizontal = 2
    };

    Mapper* mapper;
    MirroringType mirroringType;
    bool hasBatteryBackedRAM;
    bool useFourScreenMirroring;
    std::string const filepath;
    std::vector<uint8_t> prgROM;
    std::vector<uint8_t> chrROM;
    std::stack<uint8_t> capturedPRGWrites;
};



