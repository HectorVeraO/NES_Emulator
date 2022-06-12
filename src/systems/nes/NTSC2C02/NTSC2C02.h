//
// Created by junds on 27-09-2021.
//

#pragma once

#include <bitset>
#include "Canvas.h"
#include "Bus.h"

class NTSC2C02 {
public:
    NTSC2C02();
    ~NTSC2C02();

    bool nmi{ false };
    uint8_t* oamp = reinterpret_cast<uint8_t*>(oam.data());

    void clock();
    void reset();
    void loadCartridge(std::shared_ptr<Cartridge> const& newCartridge);
    void connectPlatform(Canvas* platform);

    // memory.cpp
    [[nodiscard]] uint8_t readPPUMemory(uint16_t address) const;
    void writePPUMemory(uint16_t address, uint8_t value);
    [[nodiscard]] uint8_t readIO(uint16_t address);
    void writeIO(uint16_t address, uint8_t value);

    // debugging.cpp
    std::vector<uint32_t> getNametable(uint8_t const& id) const;

private:
    Canvas* canvas{ nullptr };
    std::shared_ptr<Cartridge> cartridge{ nullptr };

    int16_t currentScanline{ -1 };
    int16_t currentCycle{ 0 };

    typedef std::vector<std::vector<uint8_t>> uint8_matrix;
    uint8_matrix nameTables{};
    uint8_matrix patternTables{};
    std::array<uint8_t, 32> palettes{};
    std::array<uint32_t, 64> rgbColors{};

    class OamEntry {
    public:
        uint8_t y;
        uint8_t tileId;
        uint8_t attributes;
        uint8_t x;
    };
    std::array<OamEntry, 64> oam;
    uint8_t oamAddr{ 0x00 };

    std::array<OamEntry, 8> scanlineSprites;
    uint8_t spriteCount{ 0 };
    uint8_t spriteShifterPatternLo[8];
    uint8_t spriteShifterPatternHi[8];

    bool spriteZeroHitPossible = false;
    bool spriteZeroBeingRendered = false;

    class MemoryMappedRegister {
    public:
        virtual explicit operator uint8_t() const = 0;
    };

    class ControllerRegister : public MemoryMappedRegister {
    public:
        std::bitset<2> NN;
        bool I{ false };
        bool S{ false };
        bool B{ false };
        bool H{ false };
        bool P{ false };
        bool V{ false };

        explicit operator uint8_t() const override {
            return (NN.to_ulong() << 0) | (I << 2) | (S << 3) << (B << 4) << (H << 5) << (P << 6) << (V << 7);
        }

        ControllerRegister& operator=(uint8_t const& rhs) {
            NN = (rhs >> 0) & 0x03;
            I = (rhs >> 2) & 0x01;
            S = (rhs >> 3) & 0x01;
            B = (rhs >> 4) & 0x01;
            H = (rhs >> 5) & 0x01;
            P = (rhs >> 6) & 0x01;
            V = (rhs >> 7) & 0x01;

            return *this;
        }
    } PPUCTRL;

    class MaskRegister : public MemoryMappedRegister {
    public:
        bool G{ false };
        bool m{ false };
        bool M{ false };
        bool b{ false };
        bool s{ false };
        std::bitset<3> BGR;

        explicit operator uint8_t() const override {
            return (G << 0) | (m << 1) | (M << 2) | (b << 3) | (s << 4) | (BGR.to_ulong() << 5);
        }

        MaskRegister& operator=(uint8_t const& rhs) {
            G = (rhs >> 0) & 0x01;
            m = (rhs >> 1) & 0x01;
            M = (rhs >> 2) & 0x01;
            b = (rhs >> 3) & 0x01;
            s = (rhs >> 4) & 0x01;
            BGR.set(0, (rhs >> 5) & 0x01);
            BGR.set(1, (rhs >> 6) & 0x01);
            BGR.set(2, (rhs >> 7) & 0x01);

            return *this;
        }
    } PPUMASK;

    class StatusRegister : public MemoryMappedRegister {
    public:
        std::bitset<5> rest;
        bool O{ false };
        bool S{ false };
        bool V{ false };

        explicit operator uint8_t() const override {
            return (rest.to_ulong() << 0) | (O << 5) | (S << 6) | (V << 7);
        }

        StatusRegister& operator=(const uint8_t& rhs) {
            rest = rhs & 0x1F;
            O = (rhs >> 5) & 0x01;
            S = (rhs >> 6) & 0x01;
            V = (rhs >> 7) & 0x01;

            return *this;
        }
    } PPUSTATUS;

    class OAMAdressRegister : public MemoryMappedRegister {
    public:
        uint8_t value{ 0x00 };

        explicit operator uint8_t() const override {
            return value;
        }

        OAMAdressRegister& operator=(const uint8_t& rhs) {
            value = rhs;

            return *this;
        }
    } OAMADDR;

    class OAMDataRegister : public MemoryMappedRegister {
    public:
        uint8_t value{ 0x00 };

        explicit operator uint8_t() const override {
            return value;
        }
    } OAMDATA;

    class ScrollRegister : MemoryMappedRegister {
    public:
        uint8_t fineOffset{ 0x00 };

        explicit operator uint8_t() const override {
            return fineOffset;
        }

        ScrollRegister& operator=(uint8_t const& rhs) {
            fineOffset = rhs;
            return *this;
        }
    } PPUSCROLL;

    class AddressRegister : MemoryMappedRegister {
    public:;
        uint8_t value{ 0x00 };

        explicit operator uint8_t() const override {
            return value;
        }

        AddressRegister& operator=(uint8_t const& rhs) {
            value = rhs;
            return *this;
        }
    } PPUADDR;

    class DataRegister : MemoryMappedRegister {
    public:
        uint8_t value{ 0x00 };

        explicit operator uint8_t() const override {
            return value;
        }

        DataRegister& operator=(uint8_t const& rhs) {
            value = rhs;
            return *this;
        }
    } PPUDATA;

    class DMARegister : MemoryMappedRegister {
    public:
        uint8_t value{ 0x00 };

        explicit operator uint8_t() const override {
            return value;
        }
    } OAMDMA;

    class LoopyRegister {
    private:
        class VRAMAddressRegister {
        public:
            std::bitset<5> coarseX;
            std::bitset<5> coarseY;
            std::bitset<1> nametableX;
            std::bitset<1> nametableY;
            std::bitset<3> fineY;
            std::bitset<1> unused;

            explicit operator uint16_t() const {
                return (coarseX.to_ulong() << 0) | (coarseY.to_ulong() << 5) | (nametableX.to_ulong() << 10) | (nametableY.to_ulong() << 11) | (fineY.to_ulong() << 12) | (unused.to_ulong() << 15);
            };

            VRAMAddressRegister& operator=(uint16_t const& rhs) {
                coarseX = (rhs >> 0) & 0x001F;
                coarseY = (rhs >> 5) & 0x001F;
                nametableX = (rhs >> 10) & 0x0001;
                nametableY = (rhs >> 11) & 0x0001;
                fineY = (rhs >> 12) & 0x0007;
                unused = (rhs >> 15) & 0x0001;
                return *this;
            }
        };

    public:
        VRAMAddressRegister v;  // VRAM address, maps to $2006
        VRAMAddressRegister t;  // Temporal VRAM address
        std::bitset<3> fineX;
        std::bitset<1> w;       // Shared latch
    } loopy;

    uint8_t dataBuffer{ 0x00 };

    uint8_t bgNextTileId = 0x00;
    uint8_t bgNextTileAttribute = 0x00;
    uint8_t bgNextTileLsb = 0x0;
    uint8_t bgNextTileMsb = 0x0;

    uint16_t bgShifterPatternLsb = 0x0000;
    uint16_t bgShifterPatternMsb = 0x0000;
    uint16_t bgShifterAttributeLsb = 0x0000;
    uint16_t bgShifterAttributeMsb = 0x0000;

    std::vector<uint32_t> frameBuffer{};

    [[nodiscard]] uint32_t getPixelColor(uint8_t pixel, uint8_t palette) const;
    void drawFrame();
};



