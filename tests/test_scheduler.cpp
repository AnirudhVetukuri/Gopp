#include <iostream>
#include "gopp/Scheduler.h"

using namespace gopp;

void sampleFunction1()
{
    std::cout << "Test function 1 is executing." << std::endl;
}

void sampleFunction2()
{
    std::cout << "Test function 2 is executing." << std::endl;
}

void test_scheduler()
{
    Scheduler scheduler;

    auto task1 = std::make_unique<Task>(sampleFunction1);
    auto task2 = std::make_unique<Task>(sampleFunction2);

    scheduler.addTask(std::move(task1));
    scheduler.addTask(std::move(task2));

    std::cout << "Running scheduler..." << std::endl;
    scheduler.run();
    std::cout << "Scheduler run completed." << std::endl;
}

int main()
{
    test_scheduler();
    return 0;
}
