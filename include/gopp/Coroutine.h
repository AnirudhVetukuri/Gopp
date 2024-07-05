#pragma once
#include <functional>
#include <memory>
#include "Context.h"

namespace gopp
{
    enum class CoroutineState
    {
        READY,
        RUNNING,
        SUSPENDED,
        FINISHED
    };

    class Coroutine
    {
        std::function<void()> m_function;
        ExecutionContextPtr m_context;
        ExecutionContextPtr m_caller_context;
        CoroutineState m_state;

        friend void coroutineFunctionWrapper();

    public:
        Coroutine(std::function<void()> function);
        void resume();
        static void yield();

        CoroutineState state() const { return m_state; }
        bool isFinished() const { return m_state == CoroutineState::FINISHED; }

        static thread_local Coroutine* current;
        static thread_local ExecutionContext* main_context;
    };

    using CoroutinePtr = std::unique_ptr<Coroutine>;
}