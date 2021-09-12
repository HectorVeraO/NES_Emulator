#include <MOS6502.h>
#include <NesBus.h>
#include <filesystem>

int main() {
    std::cout << std::filesystem::current_path() << "\n";
    std::ofstream logger("log-test.log");

    std::array<std::string, 5> text = {
            "Hello this is log 1",
            "Hello this is log 2",
            "Hello this is log 3",
            "Hello this is log 4",
            "Hello this is log 5"
    };

    if (logger.is_open()) {
        for (const auto &item : text) {
            logger << item << '\n';
        }
        logger.close();
    }

    return 0;
}