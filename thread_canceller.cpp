#include "thread_canceller.h"

void ThreadCanceller::Cancel() {
    IsAlive = false;
}

bool ThreadCanceller::Wait(ThreadCanceller::DurationType time, ThreadCanceller::DurationType tick) {
    auto start = std::chrono::system_clock::now();
    while (IsAlive && start + time > std::chrono::system_clock::now()) {
        std::this_thread::sleep_for(tick);
    }
    return IsAlive;
}

void ThreadCanceller::Reset() {
    IsAlive = true;
}
