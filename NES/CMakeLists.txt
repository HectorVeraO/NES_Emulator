set(HEADER_DIRS
        bus
        Cartridge
        Mapper
        MOS6502)

set(HEADER_FILES
        bus/Bus.h
        bus/RawBus.h
        bus/NesBus.h
        Cartridge/Cartridge.h
        Mapper/Mapper.h
        Mapper/Mapper0.h
        MOS6502/MOS6502.h)

set(SOURCE_FILES
        bus/Bus.cpp
        bus/RawBus.cpp
        bus/NesBus.cpp
        Cartridge/Cartridge.cpp
        Mapper/Mapper.cpp
        Mapper/Mapper0.cpp
        MOS6502/MOS6502.cpp
        MOS6502/execute.cpp)


include_directories(bus)
include_directories(Cartridge)
include_directories(Mapper)
include_directories(MOS6502)

add_library(NES_lib STATIC ${SOURCE_FILES} ${HEADER_FILES})
