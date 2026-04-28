#pragma once
#include <memory>

#include "Window.h"

namespace opticrafter {

class Engine {
   public:
    Engine();
    ~Engine();
    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;

    void run();

   private:
    std::unique_ptr<Window> m_window;
};

}  // namespace opticrafter