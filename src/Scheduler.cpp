#include "gopp/Scheduler.h"

namespace gopp
{
    void Scheduler::addTask(TaskPtr task)
    {
        m_tasks.push_back(std::move(task));
    }

    void Scheduler::run()
    {
        while (m_currentTask < m_tasks.size())
        {
            TaskPtr& task = m_tasks[m_currentTask];
            if (!task->isDone())
            {
                task->execute();
            }
            
            m_currentTask++;
        }
    }
}
