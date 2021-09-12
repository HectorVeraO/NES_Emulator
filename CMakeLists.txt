cmake_minimum_required(VERSION 3.20)
project(WDNES
        VERSION 1.0
        DESCRIPTION "NES emulator"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

include_directories(NES/bus)
include_directories(NES/Cartridge)
include_directories(NES/Mapper)
include_directories(NES/MOS6502)

add_subdirectory(NES)
add_subdirectory(Test)

add_executable(WDNES main.cpp)
target_link_libraries(WDNES NES_lib)

