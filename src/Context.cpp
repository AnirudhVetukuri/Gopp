#include "gopp/Context.h"
#include <cassert>
#include <csignal>

// Placeholder implementations for scheduleNext and runTask
void scheduleNext() {}
void runTask() {}

namespace gopp
{
    ExecutionContext::ExecutionContext(const ContextType type, ExecutionContext *linkedContext)
    {
        initialize(type, linkedContext);
    }

    void ExecutionContext::initialize(const ContextType type, ExecutionContext *linkedContext)
    {
        assert(static_cast<size_t>(type) < static_cast<size_t>(ContextType::COUNT));
        getcontext(&m_context);
        
        m_stackMemory.resize(STACK_SIZES[static_cast<size_t>(type)]);
        m_context.uc_stack.ss_sp = m_stackMemory.data();
        m_context.uc_stack.ss_size = m_stackMemory.size();
        m_context.uc_stack.ss_flags = 0;

        sigemptyset(&m_context.uc_sigmask);
        sigaddset(&m_context.uc_sigmask, SIGRTMIN);
        
        if (type == ContextType::SCHEDULER)
        {
            sigaddset(&m_context.uc_sigmask, SIGUSR1);
        }

        m_context.uc_link = (linkedContext != nullptr) ? linkedContext->getContext() : nullptr;

        if (type == ContextType::SCHEDULER)
        {
            makecontext(&m_context, &scheduleNext, 0);
        }
        else
        {
            makecontext(&m_context, &runTask, 0);
        }
    }
}
