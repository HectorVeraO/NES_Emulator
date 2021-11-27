#pragma once

#ifndef DISABLE_6502_BUGS
#define DISABLE_6502_BUGS 0
#endif

#ifndef DISABLE_6502_LOGS
#define DISABLE_6502_LOGS 0
#endif

#include <cstdint>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <optional>
#include <memory>
#include "spdlog/spdlog.h"
#include "Bus.h"

class MOS6502 {
public:
    enum Flag {
        Carry = 0,
        Zero = 1,
        InterruptDisable = 2,
        DecimalMode = 3,
        BreakCommand = 4,
        B = 5,
        Overflow = 6,           // Indicates if there's a two's complement overflow
        Negative = 7
    };

    MOS6502();
    explicit MOS6502(uint16_t const& startingPC);
    ~MOS6502();

    void loop();
    void clock();
    void nmi();
    void connectBus(Bus* newBus);
    void reset();

    [[nodiscard]] inline uint8_t getA() const {
        return A;
    }

    inline void setA(uint8_t a) {
        A = a;
    }

    [[nodiscard]] uint8_t getFlag(uint8_t offset) const;
    void setFlag(uint8_t offset, bool turnOn);

private:
    using VoidHandler = void (MOS6502::*)();
    enum AddressingMode {
        None = -1,
        Implicit = 0,
        Accumulator = 1,
        Immediate = 2,
        ZeroPage = 3,
        ZeroPageX = 4,
        ZeroPageY = 5,
        Relative = 6,
        Absolute = 7,
        AbsoluteX = 8,
        AbsoluteY = 9,
        Indirect = 10,
        IndexedIndirect = 11,
        IndirectIndexed = 12
    };
    Bus* bus;
    std::shared_ptr<spdlog::logger> logger = spdlog::get("mos6502");

    uint16_t PC;  // Program counter
    uint8_t S;    // Stack pointer
    uint8_t P;    // Status register
    uint8_t A;    // Accumulator
    uint8_t X;    // Index X
    uint8_t Y;    // Index Y

    bool on{ false };
    uint8_t opcode{};
    uint8_t opvalue{};                  // Value obtained through the addressing mode
    uint16_t opaddress{};               // Address obtained though the addressing mode
    uint8_t opcycles{};
    bool crossedPageBoundary{ false };
    bool isBranchTaken{ false };
    VoidHandler irqHandler;             // Interrupt request handler (IRQ handler)
    uint64_t totalCyclesPerformed{};

    AddressingMode addressingMode;

    [[nodiscard]] uint8_t readMemory(uint16_t address) const;
    void writeMemory(uint16_t address, uint8_t value) const;

    uint8_t pullStack();
    void pushStack(uint8_t value);

    // TODO: Make this better? I might want to log in other formats or not log at all, anyway it's not priority.
    void logForNESTest(std::string const& operationAlias, uint16_t startingPC, std::optional<uint8_t> maybeFirstOperand = std::nullopt, std::optional<uint8_t> maybeSecondOperand = std::nullopt) {
        uint8_t ppuX = 0x00;
        uint8_t ppuY = 0x00;
        std::string firstOperand = maybeFirstOperand.has_value() ? fmt::format("{:0>2X}", maybeFirstOperand.value()) : "  ";
        std::string secondOperand = maybeSecondOperand.has_value() ? fmt::format("{:0>2X}", maybeSecondOperand.value()) : "  ";
        // Logging format at http://www.qmtpro.com/~nes/misc/nestest.log
#if !(DISABLE_6502_LOGS)
        logger->info("{:0>4X}  {:0>2X} {} {}  {} ${: <27X} A:{:0>2X} X:{:0>2X} Y:{:0>2X} P:{:0>2X} SP:{:0>2X} PPU:{: >3},{: >3} CYC:{}", startingPC, opcode, firstOperand, secondOperand, operationAlias, opaddress, A, X, Y, P, S, ppuX, ppuY, totalCyclesPerformed);
#endif
    }

    // execute.cpp
    void decodeOperation();
    void executeOperation(std::string const& operationAlias, VoidHandler addressingModeHandler, VoidHandler operationHandler, uint8_t baseCycles, bool canCrossPageBoundary, bool canBranch);
    void handleIllegalOperation();
    void handleUnknownOperation() const;

    // addressingModes.cpp
    void amIMP();
    void amACC();
    void amIMM();
    void amZP();
    void amABS();
    void amREL();
    void amIND();
    void amZPX();
    void amZPY();
    void amABSX();
    void amABSY();
    void amINDX();
    void amINDY();

    // interrupts.cpp
    void interrupt(uint16_t pclAddress, uint16_t pchAddress, uint8_t preservedP);
    void interruptNONE();
    void interruptNMI();
    void interruptIRQ();
    void interruptReset();

    // operations.cpp "legal" operations
    void opADC();
    void opAND();
    void opASL();
    void opBCC();
    void opBCS();
    void opBEQ();
    void opBIT();
    void opBMI();
    void opBNE();
    void opBPL();
    void opBRK();
    void opBVC();
    void opBVS();
    void opCLC();
    void opCLD();
    void opCLI();
    void opCLV();
    void opCMP();
    void opCPX();
    void opCPY();
    void opDEC();
    void opDEX();
    void opDEY();
    void opEOR();
    void opINC();
    void opINX();
    void opINY();
    void opJMP();
    void opJSR();
    void opLDA();
    void opLDX();
    void opLDY();
    void opLSR();
    void opNOP();
    void opORA();
    void opPHA();
    void opPHP();
    void opPLA();
    void opPLP();
    void opROL();
    void opROR();
    void opRTI();
    void opRTS();
    void opSBC();
    void opSEC();
    void opSED();
    void opSEI();
    void opSTA();
    void opSTX();
    void opSTY();
    void opTAX();
    void opTAY();
    void opTSX();
    void opTXA();
    void opTXS();
    void opTYA();

    // operations.cpp "illegal" operations
    void opLAX();
    void opSAX();
    void opDCP();
    void opISB();
    void opSLO();
    void opRLA();
    void opSRE();
    void opRRA();
};