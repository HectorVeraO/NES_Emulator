cmake_minimum_required(VERSION 3.20)
project(WDNES
        VERSION 1.0
        DESCRIPTION "NES emulator"
        LANGUAGES CXX)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(NES/bus)
include_directories(NES/Cartridge)
include_directories(NES/Mapper)
include_directories(NES/MOS6502)

add_subdirectory(NES)
add_subdirectory(Test)

include(FetchContent)
FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
)
FetchContent_MakeAvailable(spdlog)
include_directories(${spdlog_SOURCE_DIR}/include ${spdlog_SOURCE_DIR})

add_executable(WDNES main.cpp)
target_link_libraries(WDNES PRIVATE NES_lib)
target_link_libraries(WDNES PRIVATE spdlog)

