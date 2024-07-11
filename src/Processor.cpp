#include "gopp/Processor.h"
#include "gopp/Devices.h"
#include <cassert>
#include <mutex>
#include <vector>
#include <iostream>

namespace gopp {

bool Processor::surrenderCoroutines(std::vector<CoroutinePtr> &stolenCoroutines, bool all) {
    std::unique_lock<std::mutex> lock(m_lock);
    size_t routinesToLeave = all ? 0 : ((1 + m_coroutines.size()) >> 1);
    while (m_coroutines.size() > routinesToLeave) {
        stolenCoroutines.emplace_back(std::move(m_coroutines.back()));
        m_coroutines.pop_back();
    }
    return !m_coroutines.empty();
}

bool Processor::hasCoroutines(bool coreIdle) {
    std::unique_lock<std::mutex> lock(m_lock);
    if (!m_coroutines.empty())
        return true;
    lock.unlock();
    return false;
}

void Processor::nextCoroutine(CoroutinePtr &current, CoroutinePtr &next) {
    next.reset();
    bool notIdle = current && !current->done();
    if (!hasCoroutines(not notIdle))
        return;
    
    std::unique_lock<std::mutex> lock(m_lock);
    if (!m_coroutines.empty()) {
        next.swap(m_coroutines.front());
        m_coroutines.pop_front();
        if (notIdle) {
            // Put the current coroutine back at the end.
            m_coroutines.emplace_back(std::move(current));
        }
    }
}

void Processor::submitCoroutine(CoroutinePtr &&coroutine) {
    std::unique_lock<std::mutex> lock(m_lock);
    m_coroutines.emplace_back(std::move(coroutine));
}

} 
