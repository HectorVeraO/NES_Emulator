cmake_minimum_required(VERSION 3.20)
project(NES_Emulator)

set(CMAKE_CXX_STANDARD 14)

include_directories(source/bus)
include_directories(source/processor/MOS6502)

add_executable(NES_Emulator
        source/bus/Bus.cpp
        source/bus/Bus.h
        source/processor/MOS6502/MOS6502.cpp
        source/processor/MOS6502/MOS6502.h
        main.cpp source/nes/NES.cpp source/nes/NES.h source/processor/MOS6502/execute.cpp)
