//
// Created by junds on 06-09-2021.
//

#pragma once


#include <string>
#include <vector>
#include <stack>
#include <fstream>
#include <iostream>
#include <memory>
#include "../Mapper/Mapper.h"

class Cartridge {
public:
    enum class MirroringType {
        Undefined = 0,
        Vertical = 1,
        Horizontal = 2,
        OneScreenLo = 3,
        OneScreenHi = 4
    };

    explicit Cartridge(const std::string& filePath);
    virtual ~Cartridge();

    [[nodiscard]] bool isValid() const;

    [[nodiscard]] uint8_t readPRGMemory(uint16_t address) const;
    void writePRGMemory(uint16_t address, uint8_t value);

    [[nodiscard]] uint8_t readCHRMemory(uint16_t address) const;
    void writeCHRMemory(uint16_t address, uint8_t value);

    [[nodiscard]] Cartridge::MirroringType getMirroring() const;

    void reset();

private:
    struct INESHeader {
        char system[3];
        uint8_t format[1];
        uint8_t prgRomBankCount;
        uint8_t chrRomBankCount;
        uint8_t romControl1;
        uint8_t romControl2;
        uint8_t ramBankCount;   // 8 KB RAM banks, if 0 assume 1 page
        char unused[7];
    } header{};

    bool isValidROM{ false };

    uint8_t mapperId{0};
    uint8_t prgMemoryBankCount{0};
    uint8_t chrMemoryBankCount{0};

    std::vector<uint8_t> prgRom{};
    std::vector<uint8_t> chrRom{};
    std::stack<uint8_t> capturedPrgWrites{};
    std::shared_ptr<Mapper> mapper{ nullptr };

    MirroringType mirroring{ MirroringType::Undefined };
    bool useFourScreenMirroring{ false };
    bool hasBatteryBackedRAM{ false };
};



