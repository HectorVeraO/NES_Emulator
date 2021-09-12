//
// Created by junds on 10-09-2021.
//

#include "gmock/gmock.h"
#include "Bus.h"

#include <utility>
#include "RawBus.h"

class MockBus : public Bus {
public:
    MockBus(): bus(R"(C:\repos\WDNES\Test\resources\nestest.nes)") {};  // FIXME: Using absolute path sucks, find a way to use relative path
    explicit MockBus(std::vector<uint8_t> instructions): bus(std::move(instructions)) {};

    MOCK_METHOD(uint8_t, readMemory, (uint16_t address), (const, override));
    MOCK_METHOD(void, writeMemory, (uint16_t address, uint8_t value), (override));

    void delegateToRawBus() {
        ON_CALL(*this, readMemory).WillByDefault([this](uint16_t address) {
            return bus.readMemory(address);
        });
        ON_CALL(*this, writeMemory).WillByDefault([this](uint16_t address, uint8_t value) {
            if (address == 0x0002) {
                std::cerr << "02h -> " << unsigned(value) << "\n";
            }
            if (address == 0x0003) {
                std::cerr << "03h -> " << unsigned(value) << "\n";
            }

            return bus.writeMemory(address, value);
        });
    }

private:
    RawBus bus;
};
