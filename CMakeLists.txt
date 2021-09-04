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
        main.cpp NES/nes/NES.cpp NES/nes/NES.h NES/MOS6502/execute.cpp)
