#pragma once
#include <functional>
#include <memory>
#include "Context.h"

namespace gopp
{
    class Task
    {
        std::function<void()> m_function;
        ExecutionContextPtr m_taskContext;
        ExecutionContextPtr m_schedulerContext;
        bool m_isDone{false};

    public:
        Task(std::function<void()> function);
        void execute();
        bool isDone() const { return m_isDone; }
        ExecutionContextPtr &getTaskContext() { return m_taskContext; }
        ExecutionContextPtr &getSchedulerContext() { return m_schedulerContext; }
    };

    using TaskPtr = std::unique_ptr<Task>;
}
