#pragma once
#include <cstdint>
#include <thread>
#include <pthread.h>

namespace gopp
{
// Constants and macros relevant for the concurrency runtime

enum class ContextType : size_t
{
    TASK = 0,
    SCHEDULER = 1,
    COUNT = 2
};

// Determine the maximum number of concurrent threads
#ifndef MAX_CONCURRENT_THREADS
    static const size_t MAX_THREADS = std::thread::hardware_concurrency();
#else
    static const size_t MAX_THREADS = std::min(std::thread::hardware_concurrency(), std::max(1, MAX_CONCURRENT_THREADS));
#endif

// Define stack sizes for tasks and scheduler contexts
#ifndef TASK_STACK_SIZE
    static const size_t TASK_STACK = 4 * 1024 * 1024; // 4 MB
#else
    static const size_t TASK_STACK = std::max(1024 * 64, TASK_STACK_SIZE); // At least 64 KB
#endif
    static const size_t SCHEDULER_STACK = 64 * 1024; // 64 KB
    static const size_t STACK_SIZES[] = { TASK_STACK, SCHEDULER_STACK, 0 };
}
