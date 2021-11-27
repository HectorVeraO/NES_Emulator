//
// Created by junds on 27-09-2021.
//

#include "NTSC2C02.h"

NTSC2C02::NTSC2C02() {
    PPUCTRL = 0x00;
    PPUMASK = 0x00;
    PPUSTATUS = 0xA0;
    OAMADDR = 0x00;
    loopy.w.reset();
    PPUSCROLL = 0x00;
    PPUADDR = 0x00;
    PPUDATA = 0x00;

    frameBuffer.resize(61440);
    nameTables.resize(2);
    for (auto &item : nameTables)
        item.resize(1024);

    patternTables.resize(2);
    for (auto &item : patternTables)
        item.resize(4096);

    rgbColors = {
            0x757575, 0x271B8F, 0x0000AB, 0x47009F, 0x8F0077, 0xAB0013, 0xA70000, 0x7F0B00, 0x432F00, 0x004700, 0x005100, 0x003F17, 0x1B3F5F, 0x000000, 0x000000, 0x000000,
            0xBCBCBC, 0x0073EF, 0x233BEF, 0x8300F3, 0xBF00BF, 0xE7005B, 0xDB2B00, 0xCB4F0F, 0x8B7300, 0x009700, 0x00AB00, 0x00933B, 0x00838B, 0x000000, 0x000000, 0x000000,
            0xFFFFFF, 0x3FBFFF, 0x5F97FF, 0xA78BFD, 0xF77BFF, 0xFF77B7, 0xFF7763, 0xFF9B3B, 0xF3BF3F, 0x83D313, 0x4FDF4B, 0x58F898, 0x00EBDB, 0x000000, 0x000000, 0x000000,
            0xFFFFFF, 0xABE7FF, 0xC7D7FF, 0xD7CBFF, 0xFFC7FF, 0xFFC7DB, 0xFFBFB3, 0xFFDBAB, 0xFFE7A3, 0xE3FFA3, 0xABF3BF, 0xB3FFCF, 0x9FFFF3, 0x000000, 0x000000, 0x000000
    };
}

NTSC2C02::~NTSC2C02() = default;

void NTSC2C02::reset() {
    PPUCTRL = 0x00;
    PPUMASK = 0x00;
    loopy.w.reset();
    PPUSCROLL = 0x00;
    PPUADDR = 0x00;
    PPUDATA = 0x00;
}

void NTSC2C02::clock() {
    auto incrementScrollX = [&]() {
        if (PPUMASK.b || PPUMASK.s) {
            if (loopy.v.coarseX.to_ulong() == 31) {
                loopy.v.coarseX = 0;
                loopy.v.nametableX.flip();
            } else {
                loopy.v.coarseX = loopy.v.coarseX.to_ulong() + 1;
            }
        }
    };

    auto incrementScrollY = [&]() {
        if (PPUMASK.b || PPUMASK.s) {
            if (loopy.v.fineY.to_ulong() < 7) {
                loopy.v.fineY = loopy.v.fineY.to_ulong() + 1;
            } else {
                loopy.v.fineY = 0;

                switch (loopy.v.coarseY.to_ulong()) {
                    case 29: {
                        loopy.v.coarseY = 0;
                        loopy.v.nametableY.flip();
                        break;
                    }
                    case 31: {
                        loopy.v.coarseY = 0;
                        break;
                    }
                    default: {
                        loopy.v.coarseY = loopy.v.coarseY.to_ulong() + 1;
                    }
                }
            }
        }
    };

    auto transferAddressX = [&]() {
        if (PPUMASK.b || PPUMASK.s) {
            loopy.v.nametableX = loopy.t.nametableX;
            loopy.v.coarseX = loopy.t.coarseX;
        }
    };

    auto transferAddressY = [&]() {
        loopy.v.fineY = loopy.t.fineY;
        loopy.v.nametableY = loopy.t.nametableY;
        loopy.v.coarseY = loopy.t.coarseY;
    };

    auto loadBackgroundShifters = [&]() {
        bgShifterPatternLsb = (bgShifterPatternLsb & 0xFF00) | bgNextTileLsb;
        bgShifterPatternMsb = (bgShifterPatternMsb & 0xFF00) | bgNextTileMsb;
        bgShifterAttributeLsb = (bgShifterAttributeLsb & 0xFF00) | ((bgNextTileAttribute & 0b01) ? 0xFF : 0x00);
        bgShifterAttributeMsb = (bgShifterAttributeMsb & 0xFF00) | ((bgNextTileAttribute & 0b10) ? 0xFF : 0x00);
    };

    auto updateShifters = [&]() {
        if (PPUMASK.b) {
            bgShifterPatternLsb <<= 1;
            bgShifterPatternMsb <<= 1;
            bgShifterAttributeLsb <<= 1;
            bgShifterAttributeMsb <<= 1;
        }
    };

    if (currentScanline >= -1 && currentScanline < 240) {
        if (currentScanline == 0 && currentCycle == 0) {
            currentCycle = 1;
        }

        if (currentScanline == -1 && currentCycle == 1) {
            PPUSTATUS.V = false;
        }

        if ((currentCycle >= 2 && currentCycle < 258) || (currentCycle >= 321 && currentCycle < 338)) {
            updateShifters();

            switch ((currentCycle - 1) % 8) {
                case 0: {
                    // Fetch name table byte
                    loadBackgroundShifters();
                    auto v = static_cast<uint16_t>(loopy.v);
                    bgNextTileId = readPPUMemory(0x2000 | (v & 0x0FFF));
                    break;
                }
                case 2: {
                    // Fetch attribute table byte
                    auto v = static_cast<uint16_t>(loopy.v);
                    bgNextTileAttribute = readPPUMemory(0x23C0 | (v & 0x0C00) | ((v >> 4) & 0x38) | ((v >> 2) & 0x07));
                    if (loopy.v.coarseY.to_ulong() & 0x02) {
                        bgNextTileAttribute >>= 4;
                    }
                    if (loopy.v.coarseX.to_ulong() & 0x02) {
                        bgNextTileAttribute >>= 2;
                    }
                    bgNextTileAttribute &= 0x03;
                    break;
                }
                case 4: {
                    // Fetch pattern table's low plane
                    bgNextTileLsb = readPPUMemory((PPUCTRL.B << 12) + (static_cast<uint16_t>(bgNextTileId) << 4) + loopy.v.fineY.to_ulong() + 0);
                    break;
                }
                case 6: {
                    // Fetch pattern table's high plane (it's 8 bytes from the low plane)
                    bgNextTileLsb = readPPUMemory((PPUCTRL.B << 12) + (static_cast<uint16_t>(bgNextTileId) << 4) + loopy.v.fineY.to_ulong() + 8);
                    break;
                }
                case 7: {
                    incrementScrollX();
                    break;
                }
            }
        }

        if (currentCycle == 256) {
            incrementScrollY();
        }

        if (currentCycle == 257) {
            loadBackgroundShifters();
            transferAddressX();
        }

        if (currentCycle == 338 || currentCycle == 340) {
            bgNextTileId = readPPUMemory(0x2000 | (static_cast<uint16_t>(loopy.v) & 0x0FFF));
        }

        if (currentScanline == -1 && currentCycle >= 280 && currentCycle < 305) {
            transferAddressY();
        }
    }

    if (currentScanline == 240) {
        // Idle
    }

    PPUSTATUS.V = currentScanline == 241 && currentCycle == 1;
    nmi = PPUSTATUS.V && PPUCTRL.V;

    uint8_t bgPixel = 0x00;
    uint8_t bgPalette = 0x00;

    if (PPUMASK.b) {
        uint16_t bitMultiplexer = 0x8000 >> loopy.fineX.to_ulong();

        uint8_t pixelLsb = (bgShifterPatternLsb & bitMultiplexer) > 0;
        uint8_t pixelMsb = (bgShifterPatternMsb & bitMultiplexer) > 0;
        bgPixel = (pixelMsb << 1) | pixelLsb;

        uint8_t paletteLsb = (bgShifterAttributeLsb & bitMultiplexer) > 0;
        uint8_t paletteMsb = (bgShifterAttributeMsb & bitMultiplexer) > 0;
        bgPalette = (paletteMsb << 1) | paletteLsb;
    }

    uint16_t xpos = currentCycle - 1;
    uint16_t ypos = currentScanline;
    if (ypos >= 0 && ypos < 240 && xpos >= 0 && xpos < 256) {
        frameBuffer[(ypos * 256) + xpos] = getPixelColor(bgPixel, bgPalette);
    }

    if (xpos == 255 && ypos == 239) {
        drawFrame();
    }

    currentCycle++;
    if (currentCycle >= 341) {
        currentCycle = 0;
        currentScanline++;
        if (currentScanline >= 261) {
            currentScanline = -1;
        }
    }
}

void NTSC2C02::loadCartridge(std::shared_ptr<Cartridge> const& newCartridge) {
    cartridge = newCartridge;
}

uint32_t NTSC2C02::getPixelColor(uint8_t pixel, uint8_t palette) const {
    return rgbColors[readPPUMemory(0x3F00 + (palette << 2) + pixel) & 0x3F];
}

void NTSC2C02::drawFrame() {
    canvas->drawFrame(frameBuffer.data());
}

void NTSC2C02::connectPlatform(Canvas* platform) {
    canvas = platform;
}
