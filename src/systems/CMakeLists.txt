message("src/systems/CMakeLists ${dummy}")

set(LibNesHeaders
        nes/bus/Bus.h
        nes/bus/NesBus.h
        nes/Cartridge/Cartridge.h
        nes/Mapper/Mapper.h
        nes/Mapper/Mapper0.h
        nes/MOS6502/MOS6502.h
        nes/NTSC2C02/NTSC2C02.h)

set(LibNesSources
        nes/bus/Bus.cpp
        nes/bus/NesBus.cpp
        nes/Cartridge/Cartridge.cpp
        nes/Mapper/Mapper.cpp
        nes/Mapper/Mapper0.cpp
        nes/MOS6502/MOS6502.cpp
        nes/MOS6502/addressingModes.cpp
        nes/MOS6502/execute.cpp
        nes/MOS6502/interrupts.cpp
        nes/MOS6502/operations.cpp
        nes/NTSC2C02/NTSC2C02.cpp
        nes/NTSC2C02/debugging.cpp
        nes/NTSC2C02/memory.cpp)

add_library(LibNes STATIC ${LibNesSources} ${LibNesHeaders})

target_include_directories(LibNes PUBLIC
        nes/bus
        nes/Cartridge
        nes/Mapper
        nes/MOS6502
        nes/NTSC2C02)
#        "${LibCanvasInclude}")
#target_include_directories(LibNes PRIVATE
#        "${spdlog_SOURCE_DIR}/include")

target_link_libraries(LibNes PUBLIC LibCanvas)
target_link_libraries(LibNes PRIVATE spdlog)
