#include "opticrafter/ThreadPool.h"

namespace opticrafter {

ThreadPool::ThreadPool(size_t num_threads) {
    for (size_t i = 0; i < num_threads; i++) {
        m_threads.emplace_back([this] {
            while (true) {
                std::function<void()> task;

                {
                    // Lock the pool
                    std::unique_lock<std::mutex> lock(m_mutex);

                    // Wait for pending task or pool stop
                    m_cv.wait(lock, [this] { return !m_tasks.empty() || m_stop; });

                    // Stop the trhread id stop flag and task queue empty
                    if (m_stop && m_tasks.empty()) return;

                    // Recover the task
                    task = std::move(m_tasks.front());
                    m_tasks.pop();
                }  // Unlock the pool

                // Run the task
                task();
            }
        });
    }
}

ThreadPool::~ThreadPool() {
    {
        // Lock the pool
        std::unique_lock<std::mutex> lock(m_mutex);
        m_stop = true;
    }

    // Notifify all the threads
    m_cv.notify_all();

    // Joining all the threads to ensure they have completed their tasks
    for (auto& thread : m_threads) {
        thread.join();
    }
}

void ThreadPool::enqueue(std::function<void()> task) {
    {
        // Lock the pool
        std::unique_lock<std::mutex> lock(m_mutex);
        m_tasks.push(std::move(task));
    }

    m_cv.notify_all();
}

}  // namespace opticrafter