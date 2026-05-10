#pragma once

#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace opticrafter {

class ThreadPool {
   public:
    ThreadPool(size_t num_threads = std::thread::hardware_concurrency());
    ~ThreadPool();

    template <typename F, typename... Args>
    std::future<std::invoke_result_t<F, Args...>> enqueue(F&& f, Args&&... args) {
        // Package task wraps a promise (from the worker thread)
        // and a future (to the main thread)
        using ReturnType = std::invoke_result_t<F, Args...>;

        // Here we create a task that translate function parameters to lambda
        // (to enqueue the task with a signature <void()> in m_tasks)
        auto task = std::make_shared<std::packaged_task<ReturnType()>>(
            [f = std::forward<F>(f), ... args = std::forward<Args>(args)]() mutable {
                return f(std::forward<Args>(args)...);
            });

        auto future = task->get_future();
        {
            // Lock the pool and push the task
            std::unique_lock lock(m_mutex);
            m_tasks.push([task]() { (*task)(); });
        }

        m_cv.notify_one();
        return future;
    }

   private:
    std::vector<std::thread> m_threads;
    std::queue<std::function<void()>> m_tasks;
    std::mutex m_mutex;
    std::condition_variable m_cv;
    bool m_stop = false;
};

}  // namespace opticrafter