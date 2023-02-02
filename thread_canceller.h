#pragma once

#include <atomic>
#include <future>
#include <chrono>

class ThreadCanceller {
public:
    using DurationType = std::chrono::milliseconds;
    ThreadCanceller() = default;
    void Cancel();
    bool Wait(DurationType time = DurationType(0), DurationType tick = DurationType(10));
    void Reset();

private:
    //std::promise<void> Promise;
    //std::future<void> Future;
    std::atomic<bool> IsAlive = true;
};
