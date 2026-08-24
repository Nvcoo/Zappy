/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Parser
*/

#include "../../include/protocol/Parser.hpp"
#include <sstream>

static int parseId(const std::string &s)
{
    if (!s.empty() && s[0] == '#')
        return std::stoi(s.substr(1));
    return std::stoi(s);
}

std::vector<std::string> Parser::split(const std::string &line) const
{
    std::istringstream stream(line);
    std::vector<std::string> tokens;
    std::string token;

    while (stream >> token)
        tokens.push_back(token);
    return tokens;
}

void Parser::handleMsz(const std::vector<std::string> &args, World &world) const
{
    if (args.size() < 2)
        return;
    world.setSize(std::stoi(args[0]), std::stoi(args[1]));
}

void Parser::handleBct(const std::vector<std::string> &args, World &world) const
{
    if (args.size() < 9)
        return;
    int x = std::stoi(args[0]);
    int y = std::stoi(args[1]);
    Tile &tile = world.getTile(x, y);

    for (int r = 0; r < RESOURCE_COUNT; r++)
        tile.setResource(static_cast<Resource>(r), std::stoi(args[2 + r]));
}

void Parser::handleTna(const std::vector<std::string> &args, World &world) const
{
    if (args.empty())
        return;
    world.addTeam(args[0]);
}

void Parser::handlePnw(const std::vector<std::string> &args, World &world) const
{
    if (args.size() < 6)
        return;
    int id = parseId(args[0]);
    int x = std::stoi(args[1]);
    int y = std::stoi(args[2]);
    orientation_t o = static_cast<orientation_t>(std::stoi(args[3]));
    int level = std::stoi(args[4]);

    world.addPlayer(id, x, y, o, level, args[5]);
}

void Parser::handlePpo(const std::vector<std::string> &args, World &world) const
{
    if (args.size() < 4)
        return;
    int id = parseId(args[0]);
    int x = std::stoi(args[1]);
    int y = std::stoi(args[2]);
    orientation_t o = static_cast<orientation_t>(std::stoi(args[3]));

    world.movePlayer(id, x, y, o);
}

void Parser::handlePlv(const std::vector<std::string> &args, World &world) const
{
    if (args.size() < 2)
        return;
    world.setPlayerLevel(parseId(args[0]), std::stoi(args[1]));
}

void Parser::handlePdi(const std::vector<std::string> &args, World &world) const
{
    if (args.empty())
        return;
    world.removePlayer(parseId(args[0]));
}

void Parser::handleEnw(const std::vector<std::string> &args, World &world) const
{
    if (args.size() < 4)
        return;
    int eggId = parseId(args[0]);
    int x = std::stoi(args[2]);
    int y = std::stoi(args[3]);

    world.addEgg(eggId, x, y);
}

void Parser::handleEggRemoval(const std::vector<std::string> &args, World &world) const
{
    if (args.empty())
        return;
    world.removeEgg(parseId(args[0]));
}

void Parser::parseLine(const std::string &line, World &world) const
{
    std::vector<std::string> tokens = split(line);

    if (tokens.empty())
        return;

    std::string cmd = tokens[0];
    std::vector<std::string> args(tokens.begin() + 1, tokens.end());
    auto it = ParserCommandMap.find(cmd);

    if (it == ParserCommandMap.end())
        return;
    switch (it->second) {
        case MSZ: handleMsz(args, world); break;
        case BCT: handleBct(args, world); break;
        case TNA: handleTna(args, world); break;
        case PNW: handlePnw(args, world); break;
        case PPO: handlePpo(args, world); break;
        case PLV: handlePlv(args, world); break;
        case PDI: handlePdi(args, world); break;
        case ENW: handleEnw(args, world); break;
        case EBO: handleEggRemoval(args, world); break;
        case EDI: handleEggRemoval(args, world); break;
    }
}
