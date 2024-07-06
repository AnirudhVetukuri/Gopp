#pragma once
#include <vector>
#include <mutex>
#include <condition_variable>
#include <unordered_map>
#include <atomic>
#include <thread>
#include <functional>
#include <memory>
#include <future>
#include <queue>
#include "Coroutine.h"
#include "Processor.h"
#include "Scheduler.h"

namespace gopp
{
    class Devices
    {
        std::vector<CoroutinePtr> m_globalQueue;
        std::mutex m_globalQueueMutex;
        std::condition_variable m_newCoroutineCV;

        std::vector<ProcessorPtr> m_idleProcessors;
        std::mutex m_processorMutex;
        std::condition_variable m_idleProcCV;

        std::atomic_bool m_stopped{false};
        std::unordered_map<std::thread::id, ExecutionContextPtr> m_schedulers;

        // Singleton instance.
        static Devices* s_instance;

        Devices();
        ~Devices();

    public:
        Devices(const Devices&) = delete;
        Devices &operator=(const Devices&) = delete;

        static Devices* getInstance();

        // Submit a new coroutine into the global queue.
        template <typename Fn, typename... Args>
        void submitCoroutine(Fn &&fn, Args &&...args)
        {
            std::packaged_task<void()> task(std::bind(std::forward<Fn>(fn), std::forward<Args>(args)...));
            auto coroutine = std::make_unique<Coroutine>(std::move(task));
            {
                std::lock_guard<std::mutex> lock(m_globalQueueMutex);
                m_globalQueue.push_back(std::move(coroutine));
            }
            m_newCoroutineCV.notify_one();
        }

        // Move work from the global queue into a processor.
        void pullProcessor(ProcessorPtr &processor);

        // Transfer coroutines from the global queue or other processors.
        void pullCoroutines(std::vector<CoroutinePtr> &coroutines, int stealerId, bool coreIdle);

        void finalize();
        bool running() const { return !m_stopped.load(); }

        // Static helpers for scheduling operations.
        static void runCoroutine();
        static void scheduleNext();
        static void yieldToScheduler();
        static void yieldCoroutinesAndProcessor();
        static void sigUsrHandler(int signal);
        static void timerInterruptHandler(int sig, siginfo_t *si, void *uc);
    };

    // A simple spin-lock wrapper that yields while waiting.
    template <typename Lock>
    class SpinYieldLock
    {
        Lock &m_lock;
    public:
        explicit SpinYieldLock(Lock &lock) : m_lock(lock)
        {
            while (!m_lock.try_lock())
            {
                yieldToScheduler();
            }
        }
        ~SpinYieldLock() { m_lock.unlock(); }
    };

    inline void yieldToScheduler()
    {
        // Minimal yield implementation.
        std::this_thread::yield();
    }
}

// Helper function to easily submit a coroutine.
template <typename Fn, typename... Args>
void go(Fn &&fn, Args &&...args)
{
    gopp::Devices::getInstance()->submitCoroutine(std::forward<Fn>(fn), std::forward<Args>(args)...);
}
