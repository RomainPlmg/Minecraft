#include <opticrafter/OptiCrafter.h>

#include "WorldLayer.h"

int main() {
    std::unique_ptr<opticrafter::Engine> engine;

    try {
        engine = std::make_unique<opticrafter::Engine>();
        opticrafter::Logger::setAppLoggerLevel(opticrafter::LogLevel::TRACE);
        engine->pushLayer<WorldLayer>(*engine->renderer());
        engine->run();
    } catch (const std::exception& e) {
        LOG_ERROR("Exception throwed: {}", e.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
