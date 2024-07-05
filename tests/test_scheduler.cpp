#include <iostream>
#include "gopp/Scheduler.h"

using namespace gopp;

void coroutineFunction1()
{
    std::cout << "Coroutine 1: Part 1" << std::endl;
    Coroutine::current->yield();
    std::cout << "Coroutine 1: Part 2" << std::endl;
}

void coroutineFunction2()
{
    std::cout << "Coroutine 2: Part 1" << std::endl;
    Coroutine::current->yield();
    std::cout << "Coroutine 2: Part 2" << std::endl;
}

void test_scheduler()
{
    const size_t numThreads = 4;
    Scheduler scheduler(numThreads);

    auto coroutine1 = std::make_unique<Coroutine>(coroutineFunction1);
    auto coroutine2 = std::make_unique<Coroutine>(coroutineFunction2);

    scheduler.addTask(std::move(coroutine1));
    scheduler.addTask(std::move(coroutine2));

    std::cout << "Running scheduler with " << numThreads << " threads..." << std::endl;
    scheduler.run();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Scheduler run completed." << std::endl;
}

int main()
{
    test_scheduler();
    return 0;
}
