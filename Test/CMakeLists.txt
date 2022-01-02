project(WDNES_test)
add_subdirectory(lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(${gmock_SOURCE_DIR}/include ${gmock_SOURCE_DIR})
include_directories(${spdlog_SOURCE_DIR}/include ${spdlog_SOURCE_DIR})

include_directories(NES/bus)

add_executable(WDNES_test_run main.cpp NES/bus/MockBus.h NES/MOS6502/TestMOS6502.cpp main.cpp)
target_link_libraries(WDNES_test_run NES_LIB)
target_link_libraries(WDNES_test_run gtest gmock)
