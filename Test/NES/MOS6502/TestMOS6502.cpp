//
// Created by junds on 10-09-2021.
//

#include <NesBus.h>
#include "gtest/gtest.h"
#include "MockBus.h"
#include "MOS6502.h"

TEST(MOS6502, nestest) {
//    testing::NiceMock<MockBus> bus;
//    bus.delegateToRawBus();
    NesBus bus;
    bus.connectCartridge(R"(C:\repos\WDNES\Test\resources\nestest.nes)");

    MOS6502 cpu(0xC000);
    cpu.connectBus(&bus);
    cpu.loop();
}
