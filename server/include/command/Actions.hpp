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

namespace command {

void moveForward(game::Player &player, world::Map &map);
void rotateLeft(game::Player &player);
void rotateRight(game::Player &player);
std::string executeInventory(game::Player &player);

}

#endif
