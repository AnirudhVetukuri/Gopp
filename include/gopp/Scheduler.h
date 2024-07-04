#pragma once
#include <vector>
#include <memory>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include "Task.h"

namespace gopp
{
    class Scheduler
    {
        std::vector<std::thread> m_workers;
        std::queue<TaskPtr> m_taskQueue;
        std::mutex m_queueMutex;
        std::condition_variable m_condition;
        bool m_stop{false};

        void workerThread();

    public:
        Scheduler(size_t numThreads);
        ~Scheduler();

        void addTask(TaskPtr task);
        void run();
    };

    using SchedulerPtr = std::unique_ptr<Scheduler>;
}
