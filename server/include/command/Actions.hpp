/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Actions
*/

#ifndef ACTIONS_HPP_
    #define ACTIONS_HPP_

#include "../game/Player.hpp"
#include "../world/Map.hpp"
#include "../world/Team.hpp"
#include <string>
#include <vector>

namespace command {

void moveForward(game::Player &player, Map &map);
void rotateLeft(game::Player &player);
void rotateRight(game::Player &player);
std::string executeInventory(game::Player &player);
std::string executeTake(game::Player &player, Map &map, const std::string &arg);
std::string executeSet(game::Player &player, Map &map, const std::string &arg);
std::string executeConnectNbr(game::Player &player, std::vector<Team> &teams);

Resource nameToResource(const std::string &name);

}

#endif
