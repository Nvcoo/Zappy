#pragma once
#include <string>
#include <vector>
#include "world/World.hpp"

class Parser {
public:
    Parser() = default;
    bool parse(const std::string &line, World &world);

private:
    // helpers
    std::vector<std::string> splitTokens(const std::string &s);
    int safeStoi(const std::string &s, int fallback = 0);
    int parseHashInt(const std::string &s, int fallback = 0); // "#12" -> 12
    std::string joinFrom(const std::vector<std::string> &toks, size_t start);
};
