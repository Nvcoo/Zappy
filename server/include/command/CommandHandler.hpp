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

namespace command {

game::Command parseCommand(const std::string &line);
int getCommandCost(CommandType type);
std::string executeCommand(game::Player &player, const game::Command &cmd, world::Map &map);
void processPlayerCommands(game::Player &player, game::Clock &clock, world::Map &map);

}

#endif
