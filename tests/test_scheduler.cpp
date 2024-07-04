#include <iostream>
#include "gopp/Scheduler.h"

using namespace gopp;

void sampleFunction1()
{
    std::cout << "Sample function 1 is executing." << std::endl;
}

void sampleFunction2()
{
    std::cout << "Sample function 2 is executing." << std::endl;
}

void test_scheduler()
{
    const size_t numThreads = 4;
    Scheduler scheduler(numThreads);

    auto task1 = std::make_unique<Task>(sampleFunction1);
    auto task2 = std::make_unique<Task>(sampleFunction2);

    scheduler.addTask(std::move(task1));
    scheduler.addTask(std::move(task2));

    std::cout << "Running scheduler with " << numThreads << " threads..." << std::endl;
    scheduler.run();
    std::this_thread::sleep_for(std::chrono::seconds(1)); // Wait for tasks to complete
    std::cout << "Scheduler run completed." << std::endl;
}

int main()
{
    test_scheduler();
    return 0;
}
