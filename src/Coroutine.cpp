#include "gopp/Coroutine.h"
#include <iostream>

namespace gopp
{
    thread_local Coroutine* Coroutine::current = nullptr;
    thread_local ExecutionContext* Coroutine::main_context = nullptr;

    void coroutineFunctionWrapper()
    {
        if (Coroutine::current)
        {
            Coroutine::current->m_function();
            Coroutine::current->m_state = CoroutineState::FINISHED;
            Coroutine::yield();
        }
    }

    Coroutine::Coroutine(std::function<void()> function)
        : m_function(std::move(function)), m_state(CoroutineState::READY)
    {
        m_context = std::make_unique<ExecutionContext>(ContextType::TASK);
        m_caller_context = std::make_unique<ExecutionContext>(ContextType::TASK);
        
        auto ctx = m_context->getContext();
        makecontext(ctx, reinterpret_cast<void(*)()>(coroutineFunctionWrapper), 0);
    }

    void Coroutine::resume()
    {
        if (m_state == CoroutineState::FINISHED) return;

        if (!main_context)
        {
            main_context = new ExecutionContext(ContextType::TASK);
            getcontext(main_context->getContext());
        }

        Coroutine* previous = current;
        current = this;
        m_state = CoroutineState::RUNNING;

        if (previous)
        {
            swapcontext(previous->m_caller_context->getContext(), m_context->getContext());
        }
        else
        {
            swapcontext(main_context->getContext(), m_context->getContext());
        }

        if (m_state != CoroutineState::FINISHED) m_state = CoroutineState::SUSPENDED;
    }

    void Coroutine::yield()
    {
        if (current)
        {
            Coroutine* yielding = current;
            yielding->m_state = CoroutineState::SUSPENDED;
            current = nullptr;

            if (yielding->m_state == CoroutineState::FINISHED)
            {
                swapcontext(yielding->m_context->getContext(), main_context->getContext());
            }
            else
            {
                swapcontext(yielding->m_context->getContext(), yielding->m_caller_context->getContext());
            }
        }
    }
}