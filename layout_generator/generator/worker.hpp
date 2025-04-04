#ifndef WORKER_HPP
#define WORKER_HPP

#include <concepts>
#include <condition_variable>

namespace finger_tracking {

template <typename TaskFunc, typename T>
    requires std::invocable<TaskFunc, T&>
struct Worker {
    explicit Worker(TaskFunc task)
        : m_task{task} {}

    Worker(Worker const& other)
        : m_task{other.m_task} {}
    Worker(Worker&&) = default;

    ~Worker() { stop(); }

    void run() {
        m_isStopped.store(false, std::memory_order_release);

        do {
            std::unique_lock lock(m_mutex);
            m_valueReady.wait(lock, [this] {
                return m_isStopped.load(std::memory_order_acquire) || m_value != nullptr;
            });

            if (m_isStopped.load(std::memory_order_acquire))
                return;

            T* value = std::exchange(m_value, nullptr);
            lock.unlock();

            m_isProcessing.store(true, std::memory_order_release);
            m_task(*value);
            m_isProcessing.store(false, std::memory_order_release);

            m_processingDone.notify_one();
        } while (true);
    }

    void blockWhileWaiting() {
        std::unique_lock lock(m_mutex);
        m_processingDone.wait(lock, [this] {
            return !m_isProcessing.load(std::memory_order_acquire);
        });
    }

    void post(T& value) {
        {
            std::unique_lock lock(m_mutex);
            m_value = &value;
        }
        m_valueReady.notify_one();
    }

    void stop() {
        m_isStopped.store(true, std::memory_order_release);
        {
            std::unique_lock lock(m_mutex);
            m_value = nullptr;
        }
        m_valueReady.notify_one();
    }

private:
    TaskFunc                m_task;
    T*                      m_value = nullptr;
    std::condition_variable m_valueReady;
    std::condition_variable m_processingDone;
    std::atomic_bool        m_isStopped{true};
    std::atomic_bool        m_isProcessing{false};
    std::mutex              m_mutex;
};

} // namespace finger_tracking

#endif // WORKER_HPP
