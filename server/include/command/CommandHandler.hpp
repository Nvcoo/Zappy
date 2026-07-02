/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** CommandHandler
*/

#ifndef COMMANDHANDLER_HPP_
    #define COMMANDHANDLER_HPP_

#include "../game/Player.hpp"
#include "../game/Clock.hpp"
#include "../world/Map.hpp"
#include "CommandType.hpp"
#include "../world/Team.hpp"
#include <vector>

namespace command {

game::Command parseCommand(const std::string &line);
int getCommandCost(CommandType type);
std::string executeCommand(game::Player &player, const game::Command &cmd, Map &map, std::vector<Team> &teams);
void processPlayerCommands(game::Player &player, game::Clock &clock, Map &map, std::vector<Team> &teams);

}

#endif
