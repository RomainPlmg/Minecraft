#pragma once

#include <cstdint>

namespace opticrafter {

class Timer {
   public:
    void reset();
    void update();

    /* Getters */
    float dt() { return m_dt; }  // Return delta time in second

   private:
    uint64_t m_last_time = 0;  // Time since last call
    float m_dt = 0.f;          // Elapsed time between two frames in seconds
};

}  // namespace opticrafter