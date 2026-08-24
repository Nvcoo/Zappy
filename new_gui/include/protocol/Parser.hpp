/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Parser
*/

#ifndef PARSER_HPP_
    #define PARSER_HPP_

#include <string>
#include <unordered_map>
#include <vector>
#include "../world/World.hpp"

enum ParserCommand {
    MSZ,
    BCT,
    TNA,
    PNW,
    PPO,
    PLV,
    PDI,
    ENW,
    EBO,
    EDI
};

inline const std::unordered_map<std::string, ParserCommand> ParserCommandMap = {
    {"msz", MSZ},
    {"bct", BCT},
    {"tna", TNA},
    {"pnw", PNW},
    {"ppo", PPO},
    {"plv", PLV},
    {"pdi", PDI},
    {"enw", ENW},
    {"ebo", EBO},
    {"edi", EDI}
};

class Parser {
    private:
        std::vector<std::string> split(const std::string &line) const;
        void handleMsz(const std::vector<std::string> &args, World &world) const;
        void handleBct(const std::vector<std::string> &args, World &world) const;
        void handleTna(const std::vector<std::string> &args, World &world) const;
        void handlePnw(const std::vector<std::string> &args, World &world) const;
        void handlePpo(const std::vector<std::string> &args, World &world) const;
        void handlePlv(const std::vector<std::string> &args, World &world) const;
        void handlePdi(const std::vector<std::string> &args, World &world) const;
        void handleEnw(const std::vector<std::string> &args, World &world) const;
        void handleEggRemoval(const std::vector<std::string> &args, World &world) const;
    protected:
    public:
        Parser() = default;
        void parseLine(const std::string &line, World &world) const;
};

#endif
