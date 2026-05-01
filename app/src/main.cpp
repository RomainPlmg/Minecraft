#include <opticrafter/OptiCrafter.h>

#include "DebugLayer.h"
#include "WorldLayer.h"

int main() {
    std::unique_ptr<opticrafter::Engine> engine;

    try {
        engine = std::make_unique<opticrafter::Engine>();
        opticrafter::Logger::setAppLoggerLevel(opticrafter::LogLevel::TRACE);
        engine->window()->setVsync(1);
        engine->pushLayer<WorldLayer>(*engine->renderer());
        engine->pushLayer<DebugLayer>(*engine);
        engine->run();
    } catch (const std::exception& e) {
        LOG_ERROR("Exception throwed: {}", e.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
