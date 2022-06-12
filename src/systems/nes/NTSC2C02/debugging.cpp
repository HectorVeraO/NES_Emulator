//
// Created by junds on 28-11-2021.
//

#include "NTSC2C02.h"

std::vector<uint32_t> NTSC2C02::getNametable(const uint8_t& id) const {
    static uint16_t constexpr baseNametableAddressById[] = { 0x2000, 0x2400, 0x2800, 0x2C00 };
    static uint8_t constexpr tilesInRow = 32;
    static uint8_t constexpr tilesInColumn = 30;

    std::vector<uint32_t> pixels;
    pixels.resize(256 * 240);
    auto nametableAddress = baseNametableAddressById[id % 4];

    for (int i = 0; i < tilesInRow; i++) {
        for (int j = 0; j < tilesInColumn; j++) {
            auto nametableAddressOffset = (i * tilesInRow) + j;
            auto tileId = readPPUMemory(nametableAddress | nametableAddressOffset);

            for (int x = 0; x < 8; x++) {
                auto basePatternAddress = ((PPUCTRL.B ? 1 : 0) << 12) | (tileId << 4);
                auto patternLsb = readPPUMemory(basePatternAddress + x + 0);
                auto patternMsb = readPPUMemory(basePatternAddress + x + 8);

                for (int y = 0; y < 8; y++) {
                    auto lsb = (patternLsb >> (7 - y)) & 0x01;
                    auto msb = (patternMsb >> (7 - y)) & 0x01;
                    auto pattern = (msb << 1) | lsb;

                    // temp color, missing attribute data
                    uint32_t pixel = pattern > 0 ? 0xFFFFFF : 0x000000;
                    pixels.push_back(pixel);
                }
            }
        }
    }

    return pixels;
}