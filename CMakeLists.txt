cmake_minimum_required(VERSION 3.20)
project(WDNES)

set(CMAKE_CXX_STANDARD 14)

include_directories(NES/bus)
include_directories(NES/MOS6502)

add_executable(WDNES
        NES/bus/Bus.cpp
        NES/bus/Bus.h
        NES/MOS6502/MOS6502.cpp
        NES/MOS6502/MOS6502.h
        main.cpp NES/MOS6502/execute.cpp NES/Mapper/Mapper0.cpp NES/Mapper/Mapper0.h NES/Mapper/Mapper.cpp NES/Mapper/Mapper.h NES/Cartridge/Cartridge.cpp NES/Cartridge/Cartridge.h)
