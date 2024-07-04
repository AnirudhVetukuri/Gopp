#include <iostream>
#include "gopp/Task.h"

using namespace gopp;

void sampleFunction()
{
    std::cout << "Sample function is executing." << std::endl;
}

void test_task_initialization()
{
    Task task(sampleFunction);

    std::cout << "Task initialized. Executing task..." << std::endl;
    task.execute();
    std::cout << "Task execution completed." << std::endl;

    std::cout << "Task context stack size: " << task.getTaskContext()->getContext()->uc_stack.ss_size << std::endl;
    std::cout << "Scheduler context stack size: " << task.getSchedulerContext()->getContext()->uc_stack.ss_size << std::endl;
}

int main()
{
    test_task_initialization();
    return 0;
}
