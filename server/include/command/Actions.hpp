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
#include "../game/Clock.hpp"
#include <memory>
#include <string>
#include <vector>

//Movement
void moveForward(Player &player, Map &map);
void rotateLeft(Player &player);
void rotateRight(Player &player);

//Inventory
std::string executeInventory(Player &player);
std::string executeTake(Player &player, Map &map, const std::string &arg);
std::string executeSet(Player &player, Map &map, const std::string &arg);

//Information
std::string executeConnectNbr(Player &player, std::vector<Team> &teams);
std::string executeLook(Player &player, Map &map, std::vector<std::shared_ptr<Client>> &clients);

//Interaction
std::string executeBroadcast(Player &sender, const std::string &text, std::vector<std::shared_ptr<Client>> &clients, Map &map);
std::string executeFork(Player &player, std::vector<Team> &teams);
std::string executeEject(Player &player, Map &map, std::vector<std::shared_ptr<Client>> &clients);

//Incantation
void incantationStart(Player &player, Map &map, std::vector<std::shared_ptr<Client>> &clients);
std::string incantationEnd(Player &player, Map &map, std::vector<std::shared_ptr<Client>> &clients);

//Utilities
Resource nameToResource(const std::string &name);
std::string resourceToName(Resource r);

#endif
