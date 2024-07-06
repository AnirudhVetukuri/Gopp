#pragma once
#include <deque>
#include <mutex>
#include <vector>
#include <memory>
#include "Coroutine.h"

namespace gopp
{
    class Processor
    {
        mutable std::mutex m_lock;
        std::deque<CoroutinePtr> m_coroutines;
        int m_id{-1};

    public:
        explicit Processor(int id) : m_id(id) {}
        int id() const { return m_id; }

        // Add a coroutine to this processor’s queue.
        void submitCoroutine(CoroutinePtr &&coroutine);

        // Return true if processor has work available.
        bool hasCoroutines(bool coreIdle = true);

        // Work stealing: transfer some coroutines to another processor.
        bool surrenderCoroutines(std::vector<CoroutinePtr> &stolenCoroutines, bool all = false);

        // Switch out the current coroutine and fetch the next one to run.
        void nextCoroutine(CoroutinePtr &current, CoroutinePtr &next);
    };

    using ProcessorPtr = std::unique_ptr<Processor>;
}

