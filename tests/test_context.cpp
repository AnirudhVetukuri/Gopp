#include <iostream>
#include "gopp/Context.h"

using namespace gopp;

void test_context_initialization()
{
    ExecutionContext taskContext(ContextType::TASK);
    ExecutionContext schedulerContext(ContextType::SCHEDULER);

    std::cout << "Task context initialized with stack size: " << taskContext.getContext()->uc_stack.ss_size << std::endl;
    std::cout << "Scheduler context initialized with stack size: " << schedulerContext.getContext()->uc_stack.ss_size << std::endl;
}

int main()
{
    test_context_initialization();
    return 0;
}
