#include <opticrafter/OptiCrafter.h>

int main() {
    opticrafter::Engine engine;
    opticrafter::Logger::setAppLoggerLevel(opticrafter::LogLevel::TRACE);
    engine.run();
}
