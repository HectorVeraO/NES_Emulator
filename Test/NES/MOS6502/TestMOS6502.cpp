//
// Created by junds on 10-09-2021.
//

#include "gtest/gtest.h"
#include "MockBus.h"
#include "MOS6502.h"

TEST(MOS6502Instructions, ImmediateADCWithFlagsZero) {
    std::vector<uint8_t> instructions = { 0x69, 0x00 };
    testing::NiceMock<MockBus> bus(instructions);
    bus.delegateToRawBus();
    MOS6502 cpu;
    cpu.connectBus(&bus);
    cpu.clock();
    EXPECT_EQ(cpu.getA(), 0x00);
    EXPECT_EQ(cpu.getFlag(MOS6502::Flag::Carry), 0x00);
    EXPECT_EQ(cpu.getFlag(MOS6502::Flag::Zero), 0x01);
    EXPECT_EQ(cpu.getFlag(MOS6502::Flag::Overflow), 0x00);
    EXPECT_EQ(cpu.getFlag(MOS6502::Flag::Negative), 0x00);
}

TEST(MOS6502Instructions, ImmediateADCWithFlagsCarryZero) {
    std::vector<uint8_t> instructions = { 0x69, 0xFF };
    testing::NiceMock<MockBus> bus(instructions);
    bus.delegateToRawBus();
    MOS6502 cpu;
    cpu.setA(0x01);
    cpu.connectBus(&bus);
    cpu.clock();
    EXPECT_EQ(cpu.getA(), 0x00);
    EXPECT_EQ(cpu.getFlag(MOS6502::Flag::Carry), 0x01);
    EXPECT_EQ(cpu.getFlag(MOS6502::Flag::Zero), 0x01);
    EXPECT_EQ(cpu.getFlag(MOS6502::Flag::Overflow), 0x00);
    EXPECT_EQ(cpu.getFlag(MOS6502::Flag::Negative), 0x00);
}

TEST(MOS6502Instructions, ImmediateADCWithFlagsOverflowNegative) {
    std::vector<uint8_t> instructions = { 0x69, 0x7F };
    testing::NiceMock<MockBus> bus(instructions);
    bus.delegateToRawBus();
    MOS6502 cpu;
    cpu.setA(0x00);
    cpu.setFlag(MOS6502::Flag::Carry, true);
    cpu.connectBus(&bus);
    cpu.clock();
    EXPECT_EQ(cpu.getA(), 0x80);
    EXPECT_EQ(cpu.getFlag(MOS6502::Flag::Carry), 0x00);
    EXPECT_EQ(cpu.getFlag(MOS6502::Flag::Zero), 0x00);
    EXPECT_EQ(cpu.getFlag(MOS6502::Flag::Overflow), 0x01);
    EXPECT_EQ(cpu.getFlag(MOS6502::Flag::Negative), 0x01);
}


TEST(MOS6502Instructions, ImmediateSBCWithFlagsNegative) {
    std::vector<uint8_t> instructions = { 0xE9, 0x80 };
    testing::NiceMock<MockBus> bus(instructions);
    bus.delegateToRawBus();
    MOS6502 cpu;
    cpu.setA(0xFF);
    cpu.connectBus(&bus);
    cpu.clock();
    EXPECT_EQ(cpu.getA(), 0x7E);
    EXPECT_EQ(cpu.getFlag(MOS6502::Flag::Carry), 0x01);
    EXPECT_EQ(cpu.getFlag(MOS6502::Flag::Zero), 0x00);
    EXPECT_EQ(cpu.getFlag(MOS6502::Flag::Overflow), 0x00);
    EXPECT_EQ(cpu.getFlag(MOS6502::Flag::Negative), 0x00);
}

TEST(MOS6502Instructions, ImmediateSBCWithFlagsOverflowCarry) {
    std::vector<uint8_t> instructions = { 0xE9, 0x01 };
    testing::NiceMock<MockBus> bus(instructions);
    bus.delegateToRawBus();
    MOS6502 cpu;
    cpu.setA(0x80);
    cpu.setFlag(MOS6502::Flag::Carry, true);
    cpu.connectBus(&bus);
    cpu.clock();
    EXPECT_EQ(cpu.getA(), 0x7F);
    EXPECT_EQ(cpu.getFlag(MOS6502::Flag::Carry), 0x01);
    EXPECT_EQ(cpu.getFlag(MOS6502::Flag::Zero), 0x00);
    EXPECT_EQ(cpu.getFlag(MOS6502::Flag::Overflow), 0x01);
    EXPECT_EQ(cpu.getFlag(MOS6502::Flag::Negative), 0x00);
}