#include "config.h"

#include <iostream>
#include <stdexcept>
#include <sstream>
#include <string>

namespace {
std::istream& operator>>(std::istream& in, Config::DurationType& var) {
    long long s;
    in >> s;

    if (!in) {
        return in;
    }

    var = std::chrono::seconds(s);
    return in;
}

std::istream& operator>>(std::istream& in, Config::Rule& var) {
    std::string s;
    in >> s;

    if (!in) {
        return in;
    }

    size_t from = 0;
    size_t to = s.find('|', from);
    var.Directory = s.substr(from, to - from);

    from = to + 1;
    to = s.find('|', from);
    var.Mask = s.substr(from, to - from);

    from = to + 1;
    var.Age = Config::DurationType(std::stoll(s.substr(from)));
    return in;
}
}

std::unique_ptr<Config> Config::ParseFromTSV(std::ifstream file) {
    DurationType callInterval;

    file >> callInterval;

    if (!file) {
        throw std::invalid_argument("Cannot read call interval from config file");
    }

    try {
        std::vector<Config::Rule> rules;
        for (Rule rule; file >> rule;) {
            rules.push_back(std::move(rule));
        }
        return std::make_unique<Config>(Config{callInterval, rules});
    } catch (std::exception& ex) {
        throw std::invalid_argument((std::stringstream() << "Cannot read config: " << ex.what()).str());
    }
}
