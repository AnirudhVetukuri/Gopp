#pragma once
#include <vector>
#include <memory>
#include "Task.h"

namespace gopp
{
    class Scheduler
    {
        std::vector<TaskPtr> m_tasks;
        size_t m_currentTask{0};

    public:
        void addTask(TaskPtr task);
        void run();
    };

    using SchedulerPtr = std::unique_ptr<Scheduler>;
}
