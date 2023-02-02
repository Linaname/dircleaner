#pragma once

#include <chrono>
#include <filesystem>
#include <fstream>
#include <memory>
#include <string>
#include <vector>

struct Config {
    using DurationType = std::chrono::seconds;

    struct Rule {
        std::filesystem::path Directory;
        std::string Mask;
        DurationType Age;
    };

    const DurationType CallInterval;
    const std::vector<Rule> Rules;

public:
    static std::unique_ptr<Config> ParseFromTSV(std::ifstream file);
};