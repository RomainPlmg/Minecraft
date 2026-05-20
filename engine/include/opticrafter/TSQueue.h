#pragma once

#include <condition_variable>
#include <mutex>
#include <optional>
#include <queue>
#include <shared_mutex>

namespace opticrafter {

template <typename T>
class TSQueue {
   public:
    TSQueue() = default;

    void push(T data) {
        {
            std::unique_lock lock(m_mutex);
            m_queue.push(std::move(data));
        }

        m_cv.notify_one();
    }

    std::optional<T> tryPop() {
        std::unique_lock lock(m_mutex);
        if (m_queue.empty()) {
            return std::nullopt;
        }

        T data = std::move(m_queue.front());
        m_queue.pop();

        return data;
    }

    T pop() {
        std::unique_lock lock(m_mutex);
        m_cv.wait(lock, [this]() { return !m_queue.empty(); });
        T data = std::move(m_queue.front());
        m_queue.pop();

        return data;
    }

    bool empty() const {
        std::shared_lock lock(m_mutex);
        return m_queue.empty();
    }

    size_t size() const {
        std::shared_lock lock(m_mutex);
        return m_queue.size();
    }

   private:
    std::queue<T> m_queue;
    mutable std::shared_mutex m_mutex;
    std::condition_variable m_cv;
};

}  // namespace opticrafter