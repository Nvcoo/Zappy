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

void moveForward(Player &player, Map &map);
void rotateLeft(Player &player);
void rotateRight(Player &player);
std::string executeInventory(Player &player);
std::string executeTake(Player &player, Map &map, const std::string &arg);
std::string executeSet(Player &player, Map &map, const std::string &arg);
std::string executeConnectNbr(Player &player, std::vector<Team> &teams);

Resource nameToResource(const std::string &name);

#endif
