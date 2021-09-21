//
// Created by junds on 21-09-2021.
//

#include <gtest/gtest.h>
#include <spdlog/spdlog.h>

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    auto console = spdlog::stdout_color_st("mos6502");
    console->set_pattern("%v");

    return RUN_ALL_TESTS();
}