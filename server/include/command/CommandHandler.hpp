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

Command parseCommand(const std::string &line);
int getCommandCost(CommandType type);
std::string executeCommand(Player &player, const Command &cmd, Map &map, std::vector<Team> &teams);
void processPlayerCommands(Player &player, Clock &clock, Map &map, std::vector<Team> &teams);

#endif
