#pragma once
#include <thread>
#include <atomic>
#include <memory>
#include "Processor.h"
#include "Coroutine.h"
#include "Context.h"

namespace gopp
{

/*
 * Scheduler encapsulates the core scheduling functionality.
 * It manages a Processor for coroutine execution, runs a scheduling loop
 * on its dedicated thread, and handles context switching.
 */
class Scheduler
{
    ProcessorPtr m_processor;
    CoroutinePtr m_activeCoroutine;
    std::thread m_thread;
    ExecutionContextPtr m_schedulerContext;
    std::atomic_bool m_running{false};
    int m_id{-1};

public:
    // Initialize the scheduler with a given id.
    explicit Scheduler(int id);
    ~Scheduler();

    // Return the thread id of the scheduler's thread.
    std::thread::id threadId() const { return m_thread.get_id(); }
    std::thread &thread() { return m_thread; }
    int id() const { return m_id; }

    // Stop the scheduler.
    void finalize() { m_running = false; }

    // Yield processor control for cooperative multitasking.
    void yieldProcessor();

    // Main scheduling loop that continuously executes available coroutines.
    void scheduleLoop();

    // Switch execution context to the scheduler.
    void switchToScheduler();

    // Accessor for the associated processor.
    ProcessorPtr &processor() { return m_processor; }

    // Run the currently active coroutine.
    void runActiveCoroutine();
};

} // namespace gopp
