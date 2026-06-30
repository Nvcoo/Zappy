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
#include <string>

namespace command {

void moveForward(game::Player &player, world::Map &map);
void rotateLeft(game::Player &player);
void rotateRight(game::Player &player);
std::string executeInventory(game::Player &player);
std::string executeTake(game::Player &player, world::Map &map, const std::string &arg);
std::string executeSet(game::Player &player, world::Map &map, const std::string &arg);

world::Resource nameToResource(const std::string &name);

}

#endif
