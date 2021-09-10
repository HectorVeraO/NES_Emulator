cmake_minimum_required(VERSION 3.20)
project(TestWDNES)

set(CMAKE_CXX_STANDARD 14)

include(FetchContent)
FetchContent_Declare(
        googleTest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googleTest)

#add_subdirectory(${googletest_SOURCE_DIR})

#add_executable(TestWDNES
#        NES/MOS6502/TestMOS6502.cpp
#        main.cpp)
enable_testing()

add_executable(TestWDNES
        NES/MOS6502/TestMOS6502.cpp
        ../NES/bus/NesBus.cpp ../NES/bus/NesBus.h)
target_link_libraries(
        TestWDNES
        gtest_main
)

include(GoogleTest)
gtest_discover_tests(TestWDNES)