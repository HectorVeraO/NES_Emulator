project(WDNES_test)
add_subdirectory(lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(${gmock_SOURCE_DIR}/include ${gmock_SOURCE_DIR})

include_directories(NES/bus)

add_executable(WDNES_test_run NES/bus/MockBus.h NES/MOS6502/TestMOS6502.cpp)
target_link_libraries(WDNES_test_run NES_lib)
target_link_libraries(WDNES_test_run gtest gmock gtest_main)
