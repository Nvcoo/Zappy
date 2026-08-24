/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** GuiNotify
*/

#ifndef GUINOTIFY_HPP_
    #define GUINOTIFY_HPP_

#include "../game/Player.hpp"
#include "../world/Map.hpp"
#include "../world/Team.hpp"
#include "../network/GuiClient.hpp"
#include <memory>
#include <vector>
#include <functional>

void broadcastGui(std::vector<std::shared_ptr<Client>> &clients, std::function<void(GuiClient &)> fn);
void notifyForward(std::shared_ptr<Player> player, Map &map, std::vector<std::shared_ptr<Client>> &clients);
void notifyLeft(std::shared_ptr<Player> player, std::vector<std::shared_ptr<Client>> &clients);
void notifyRight(std::shared_ptr<Player> player, std::vector<std::shared_ptr<Client>> &clients);
std::string notifyTake(std::shared_ptr<Player> player, Map &map, const std::string &arg, std::vector<std::shared_ptr<Client>> &clients);
std::string notifySet(std::shared_ptr<Player> player, Map &map, const std::string &arg, std::vector<std::shared_ptr<Client>> &clients);
std::string notifyBroadcast(std::shared_ptr<Player> player, Map &map, const std::string &arg, std::vector<std::shared_ptr<Client>> &clients);
std::string notifyFork(std::shared_ptr<Player> player, std::vector<Team> &teams, std::vector<std::shared_ptr<Client>> &clients);
std::string notifyEject(std::shared_ptr<Player> player, Map &map, std::vector<Team> &teams, std::vector<std::shared_ptr<Client>> &clients);

#endif
