#include <opticrafter/OptiCrafter.h>

int main() {
    std::unique_ptr<opticrafter::Engine> engine;

    try {
        engine = std::make_unique<opticrafter::Engine>();
        opticrafter::Logger::setAppLoggerLevel(opticrafter::LogLevel::TRACE);
        engine->run();
    } catch (const std::exception& e) {
        LOG_CRITICAL("Exception throwed: {}", e.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
