//
// Created by junds on 27-09-2021.
//

#pragma once

#include <bitset>
#include "Bus.h"

class NTSC2C02 {
public:
    NTSC2C02();
    ~NTSC2C02();

    void powerUp();
    void clock();
    void connectBus(Bus* newBus);


    // memory.cpp
    [[nodiscard]] uint8_t readPPUMemory(uint16_t address) const;
    void writePPUMemory(uint16_t address, uint8_t value);
    [[nodiscard]] uint8_t readCPUMemory(uint16_t address) const;
    void writeCPUMemory(uint16_t address, uint8_t value);
private:
    Bus* bus;
    int16_t currentScanline{ -1 };
    int16_t currentCycle{ 0 };

    std::array<std::array<uint8_t, 1024>, 2> nametables{};
    std::array<uint8_t, 32> palettes{};

    class MemoryMappedRegister {
    public:
        [[nodiscard]] virtual uint16_t getAddress() const = 0;
        virtual explicit operator uint8_t() const = 0;
    };

    class ControllerRegister : public MemoryMappedRegister {
    public:
        enum class Nametable : uint8_t {
            First = 0, Second = 1, Third = 2, Fourth = 3
        };

        Nametable NN{ Nametable::First };
        bool I{ false };
        bool S{ false };
        bool B{ false };
        bool H{ false };
        bool P{ false };
        bool V{ false };

        [[nodiscard]] uint16_t getAddress() const override {
            return 0x2000;
        }

        explicit operator uint8_t() const override {
            return (static_cast<uint8_t>(NN) << 0) | (I << 2) | (S << 3) << (B << 4) << (H << 5) << (P << 6) << (V << 7);
        }

        ControllerRegister& operator=(uint8_t const& rhs) {
            NN = Nametable((rhs >> 0) & 0x03);
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

        [[nodiscard]] uint16_t getAddress() const override {
            return 0x2001;
        }

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

        [[nodiscard]] uint16_t getAddress() const override {
            return 0x2002;
        }

        explicit operator uint8_t() const override {
            return (rest.to_ulong() << 0) | (O << 5) | (S << 6) | (V << 7);
        }
    } PPUSTATUS;

    class OAMAdressRegister : public MemoryMappedRegister {
    public:
        uint8_t value{ 0x00 };

        [[nodiscard]] uint16_t getAddress() const override {
            return 0x2003;
        }

        explicit operator uint8_t() const override {
            return value;
        }
    } OAMADDR;

    class OAMDataRegister : public MemoryMappedRegister {
    public:
        uint8_t value{ 0x00 };

        [[nodiscard]] uint16_t getAddress() const override {
            return 0x2004;
        }

        explicit operator uint8_t() const override {
            return value;
        }
    } OAMDATA;

    class ScrollRegister : MemoryMappedRegister {
    public:
        std::bitset<4> fineX;
        std::bitset<4> fineY;

        [[nodiscard]] uint16_t getAddress() const override {
            return 0x2005;
        }

        explicit operator uint8_t() const override {
            return (fineX.to_ulong() << 0) | (fineY.to_ulong() << 4);   // TODO: Unsure if fineX is the first nibble
        }
    } PPUSCROLL;

    class AddressRegister : MemoryMappedRegister {
    public:;
        uint8_t value{ 0x00 };

        [[nodiscard]] uint16_t getAddress() const override {
            return 0x2006;
        }

        explicit operator uint8_t() const override {
            return value;
        }
    } PPUADDR;

    class DataRegister : MemoryMappedRegister {
    public:
        uint8_t value{ 0x00 };

        [[nodiscard]] uint16_t getAddress() const override {
            return 0x2007;
        }

        explicit operator uint8_t() const override {
            return value;
        }
    } PPUDATA;

    class DMARegister : MemoryMappedRegister {
    public:
        uint8_t value{ 0x00 };

        [[nodiscard]] uint16_t getAddress() const override {
            return 0x4014;
        }

        explicit operator uint8_t() const override {
            return value;
        }
    } OAMDMA;

    class LoopyRegister {
        class VRAMAddressRegister {
            std::bitset<5> horizontalOffset;
            std::bitset<5> verticalOffset;
            std::bitset<1> horizontalNametable;
            std::bitset<1> verticalNametable;
            std::bitset<3> tileVerticalOffset;
            std::bitset<1> unused;

            [[nodiscard]] std::bitset<12> getNametableAddressBits() const {
                return { (horizontalOffset.to_ulong() << 0) | (verticalOffset.to_ulong() << 5) | (horizontalNametable.to_ulong() << 10) | (verticalNametable.to_ulong() << 11) };
            }

            explicit operator uint16_t() const {
                return (getNametableAddressBits().to_ulong() << 0) | (tileVerticalOffset.to_ulong() << 12) | (unused.to_ulong() << 15);
            };
        };

        VRAMAddressRegister v;  // VRAM address, maps to $2006
        VRAMAddressRegister t;  // Temporal VRAM address
        std::bitset<3> x;
        std::bitset<1> w;
    };
};



