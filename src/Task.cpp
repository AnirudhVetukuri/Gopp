#include "gopp/Task.h"
#include "gopp/Context.h"

namespace gopp
{
    Task::Task(std::function<void()> function)
        : m_function(std::move(function))
    {
        m_isDone = false;

        m_schedulerContext = std::make_unique<ExecutionContext>(ContextType::SCHEDULER);
        m_taskContext = std::make_unique<ExecutionContext>(ContextType::TASK, m_schedulerContext.get());
    }

    void Task::execute()
    {
        m_function();
        m_isDone = true;
    }
}
