#pragma once

#include "thread_canceller.h"
#include "config.h"
#include "message.h"

#include <thread>
#include <future>
#include <mutex>

class Cleaner {
public:
    explicit Cleaner(std::filesystem::path configPath);
    void ReloadConfig();
    void CleanByRule(const Config::Rule& rule);

    Message PrintMessage();
    void ConsoleDialog();
    void Run();
    void Stop();
    void Help();
    void Quit();

private:
    std::filesystem::path ConfigPath;
    std::unique_ptr<Config> CurrentConfig = nullptr;
    std::thread RunThread;
    std::mutex ThreadLock;
    ThreadCanceller Canceller;
};
