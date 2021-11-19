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

    MOCK_METHOD(uint8_t, readCPUMemory, (uint16_t address), (override));
    MOCK_METHOD(void, writeCPUMemory, (uint16_t address, uint8_t value), (override));

    void delegateToRawBus() {
        ON_CALL(*this, readCPUMemory).WillByDefault([this](uint16_t address) {
            return bus.readCPUMemory(address);
        });
        ON_CALL(*this, writeCPUMemory).WillByDefault([this](uint16_t address, uint8_t value) {
            bus.writeCPUMemory(address, value);
        });
    }

private:
    RawBus bus;
};
