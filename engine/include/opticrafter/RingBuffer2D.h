#pragma once

#include <cstddef>
#include <vector>

namespace opticrafter {

template <typename T>
class RingBuffer2D {
   public:
    RingBuffer2D(size_t size) { resize(size); }

    T& get(int x, int z) { return m_data[index(x, z)]; }
    const T& get(int x, int z) const { return m_data[index(x, z)]; }

    void set(int x, int z, T&& value) { m_data[index(x, z)] = std::move(value); }

    int size() const { return m_size; }
    void resize(size_t size) {
        m_size = size;
        m_data.resize(size * size);
    }

    auto begin() const { return m_data.begin(); }
    auto end() const { return m_data.end(); }

   private:
    int m_size = 0;
    std::vector<T> m_data;

    size_t index(int x, int z) const {
        int xi = ((x % m_size) + m_size) % m_size;
        int zi = ((z % m_size) + m_size) % m_size;
        return xi + zi * m_size;
    }
};

}  // namespace opticrafter