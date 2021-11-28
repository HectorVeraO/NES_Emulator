set(NES_LIB_HEADER_DIRS
        bus
        Cartridge
        Mapper
        MOS6502
        NTSC2C02)

set(NES_LIB_HEADER_FILES
        bus/Bus.h
        bus/RawBus.h
        bus/NesBus.h
        Cartridge/Cartridge.h
        Mapper/Mapper.h
        Mapper/Mapper0.h
        MOS6502/MOS6502.h
        NTSC2C02/NTSC2C02.h)

set(NES_LIB_SOURCE_FILES
        bus/Bus.cpp
        bus/RawBus.cpp
        bus/NesBus.cpp
        Cartridge/Cartridge.cpp
        Mapper/Mapper.cpp
        Mapper/Mapper0.cpp
        MOS6502/MOS6502.cpp
        MOS6502/execute.cpp
        MOS6502/addressingModes.cpp
        MOS6502/interrupts.cpp
        MOS6502/operations.cpp
        NTSC2C02/NTSC2C02.cpp
        NTSC2C02/memory.cpp
        NTSC2C02/debugging.cpp)


include_directories(bus)
include_directories(Cartridge)
include_directories(Mapper)
include_directories(MOS6502)
include_directories(NTSC2C02)
include_directories(${spdlog_SOURCE_DIR}/include ${spdlog_SOURCE_DIR})

add_library(NES_LIB STATIC ${NES_LIB_SOURCE_FILES} ${NES_LIB_HEADER_FILES})
