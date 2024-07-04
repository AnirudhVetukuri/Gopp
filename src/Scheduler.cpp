#include "gopp/Scheduler.h"

namespace gopp
{
    Scheduler::Scheduler(size_t numThreads)
    {
        for (size_t i = 0; i < numThreads; ++i)
        {
            m_workers.emplace_back(&Scheduler::workerThread, this);
        }
    }

    Scheduler::~Scheduler()
    {
        {
            std::unique_lock<std::mutex> lock(m_queueMutex);
            m_stop = true;
        }
        m_condition.notify_all();
        for (std::thread &worker : m_workers)
        {
            if (worker.joinable())
            {
                worker.join();
            }
        }
    }

    void Scheduler::addTask(TaskPtr task)
    {
        {
            std::unique_lock<std::mutex> lock(m_queueMutex);
            m_taskQueue.push(std::move(task));
        }
        m_condition.notify_one();
    }

    void Scheduler::run()
    {
        {
            std::unique_lock<std::mutex> lock(m_queueMutex);
            m_stop = false;
        }
    }

    void Scheduler::workerThread()
    {
        while (true)
        {
            TaskPtr task;
            {
                std::unique_lock<std::mutex> lock(m_queueMutex);
                m_condition.wait(lock, [this] { return m_stop || !m_taskQueue.empty(); });
                if (m_stop && m_taskQueue.empty())
                {
                    return;
                }
                task = std::move(m_taskQueue.front());
                m_taskQueue.pop();
            }
            task->execute();
        }
    }
}
