#include "cleaner.h"

#include "wildcard_matcher.h"

#include <filesystem>
#include <iostream>
#include <map>

namespace {
    bool CompareWithMask(const std::filesystem::directory_entry& entry, const std::string& mask) {
        return IsMatch(entry.path().filename().string(), mask);
    }

    bool CompareWithAge(const std::filesystem::directory_entry& entry, Config::DurationType age) {
        return (std::filesystem::file_time_type::clock::now() - entry.last_write_time()) > age;
    }
}

Cleaner::Cleaner(std::filesystem::path configPath)
: ConfigPath(std::move(configPath))
{}

void Cleaner::ReloadConfig() {
    CurrentConfig = Config::ParseFromTSV(std::ifstream(ConfigPath));
}

void Cleaner::CleanByRule(const Config::Rule& rule) {
    if (!is_directory(rule.Directory)) {
        PrintMessage() << "Directory " << rule.Directory << " does not exist. Ignore rule.";
        return;
    }
    for (auto const& dir_entry : std::filesystem::directory_iterator{rule.Directory})
    {
        if (CompareWithMask(dir_entry, rule.Mask) && CompareWithAge(dir_entry, rule.Age)) {
            PrintMessage() << "Remove " << dir_entry.path();
            try {
                std::filesystem::remove_all(dir_entry);
            } catch (const std::exception& ex) {
                PrintMessage() << "Cannot remove: " << ex.what();
            } catch (...) {
                PrintMessage() << "Cannot remove: " << "Unknown error";
            }
        }
    }
}

void Cleaner::Run() {
    std::lock_guard guard(ThreadLock);

    if (RunThread.joinable()) {
        if (Canceller.Wait()) {
            PrintMessage() << "Already runned";
            return;
        } else {
            RunThread.join();
            Canceller.Reset();
        }
    }
    RunThread = std::thread([this](){
        bool cancelled = false;
        while (!cancelled) {
            try {
                ReloadConfig();
            } catch (std::exception& ex) {
                PrintMessage() << "Error when load config: " << ex.what() << ", thread will be stopped";
                Canceller.Cancel();
                return;
            }
            for (const auto &rule: CurrentConfig->Rules) {
                CleanByRule(rule);
            }
            PrintMessage() << "Wait " << CurrentConfig->CallInterval.count();
            cancelled = !Canceller.Wait(CurrentConfig->CallInterval);
        }
    });
}

void Cleaner::Stop() {
    std::lock_guard guard(ThreadLock);
    Canceller.Cancel();
    if (RunThread.joinable()) {
        RunThread.join();
        PrintMessage() << "Stopped";
    } else {
        PrintMessage() << "Already stopped";
    }
    Canceller.Reset();
}

Message Cleaner::PrintMessage() {
    return Message(std::cout);
}

void Cleaner::ConsoleDialog() {
    enum class Command {
        QUIT = 1,
        RUN = 2,
        STOP = 3,
        HELP = 4
    };
    std::map<std::string, Command> mapping;
    mapping["q"] = Command::QUIT;
    mapping["r"] = Command::RUN;
    mapping["s"] = Command::STOP;
    mapping["h"] = Command::HELP;
    std::string input;
    bool needQuit = false;

    Help();

    while (!needQuit) {
        std::cin >> input;
        switch (mapping[input]) {
            case Command::HELP:
                Help();
                break;
            case Command::RUN:
                Run();
                break;
            case Command::STOP:
                Stop();
                break;
            case Command::QUIT:
                needQuit = true;
                Quit();
                break;
            default:
                PrintMessage() << "Unknown command";
        }
    }
}

void Cleaner::Help() {
    PrintMessage() << "r: run, s: stop, q: quit, h: this help message";
}

void Cleaner::Quit() {
    Stop();
    PrintMessage() << "Quit";
}
