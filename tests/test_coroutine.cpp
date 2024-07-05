#include <iostream>
#include "gopp/Coroutine.h"

using namespace gopp;

void simpleCoroutineFunction()
{
    std::cout << "Simple coroutine is running." << std::endl;
    Coroutine::current->yield();
    std::cout << "Simple coroutine has resumed." << std::endl;
}

void test_coroutine()
{
    auto coroutine = std::make_unique<Coroutine>(simpleCoroutineFunction);

    std::cout << "Starting coroutine..." << std::endl;
    coroutine->resume();
    std::cout << "Coroutine yielded." << std::endl;
    coroutine->resume();
    std::cout << "Coroutine finished." << std::endl;
}

int main()
{
    test_coroutine();
    return 0;
}
