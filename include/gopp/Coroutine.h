#pragma once
#include <functional>
#include <future>
#include <memory>
#include "Context.h" 

namespace gopp
{

/*
 * Coroutine encapsulates a task submitted by the user along with its execution contexts.
 * It replaces the old cppgo Routine and serves as the new coroutine implementation.
 */
class Coroutine
{
    // Callable representing the coroutine task.
    std::packaged_task<void()> m_task;
    // Execution contexts for running the task and interfacing with the scheduler.
    ExecutionContextPtr m_runContext;
    ExecutionContextPtr m_schedulerContext;
    // Indicates whether the task has finished execution.
    bool m_done{false};

public:
    // Construct a Coroutine by moving in a packaged task.
    explicit Coroutine(std::packaged_task<void()> task)
        : m_task(std::move(task))
    {
        // Context initialization can be performed here if needed.
        // For example, one might allocate stacks or link contexts.
    }

    // Disable copying and moving.
    Coroutine(const Coroutine &) = delete;
    Coroutine &operator=(const Coroutine &) = delete;
    Coroutine(Coroutine &&) = delete;
    Coroutine &operator=(Coroutine &&) = delete;

    // Run the task if not already done.
    void run()
    {
        if (!m_done)
        {
            m_task();
            m_done = true;
        }
    }

    bool done() const { return m_done; }
    ExecutionContextPtr &runContext() { return m_runContext; }
    ExecutionContextPtr &schedulerContext() { return m_schedulerContext; }
};

using CoroutinePtr = std::unique_ptr<Coroutine>;

}
