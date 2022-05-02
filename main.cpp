#include "MOS6502.h"
#include "NesBus.h"

void runNesDemo() {
    auto now = std::chrono::system_clock::now().time_since_epoch().count();

    std::string userHomePath{ std::getenv("USERPROFILE") };
    auto baseLogDir = userHomePath + + "\\.wdnes\\logs";
    auto logName = std::to_string(now) + ".log";
    auto logPath = baseLogDir + "\\" + logName;

    // auto console = spdlog::stdout_color_st("mos6502");
    auto console = spdlog::basic_logger_mt("mos6502", logPath);
    spdlog::set_pattern("[%n] [%l] %v");
    spdlog::set_pattern("%v");

    NesBus nes;

    auto cartridge = std::make_shared<Cartridge>(R"(C:\Users\junds\Desktop\NES\ROM\nestest.nes)");
    nes.loadCartridge(cartridge);
    nes.reset();
    nes.powerUp();
}

int main(int argv, char** args) {
    for (int i = 0; i < argv; ++i)
        std::cout << args[i] << std::endl;

//    auto console = spdlog::stdout_color_st("mos6502");
//    spdlog::set_pattern("[%n] [%l] %v");
//    spdlog::set_pattern("%v");

    runNesDemo();

    return 0;
}