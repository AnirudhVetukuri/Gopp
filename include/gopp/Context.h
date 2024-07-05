#pragma once
#include "Constants.h"
#include <vector>
#include <ucontext.h>

namespace gopp
{
    class ExecutionContext
    {
        ucontext_t m_context;
        std::vector<char> m_stackMemory;

    public:
        ExecutionContext(const ContextType type, ExecutionContext *linkedContext = nullptr);
        void initialize(const ContextType type, ExecutionContext *linkedContext = nullptr);

        ucontext_t *getContext() { return &m_context; }
        std::vector<char>& getStack() { return m_stackMemory; }
    };
    using ExecutionContextPtr = std::unique_ptr<ExecutionContext>;
}
