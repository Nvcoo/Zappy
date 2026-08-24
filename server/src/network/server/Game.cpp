/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Game
*/

#include "../../../include/network/Server.hpp"
#include "../../../include/game/Player.hpp"
#include "../../../include/command/CommandHandler.hpp"
#include <iostream>

void Server::updateGame()
{
    if (_clock.respawn()) {
        auto changedTiles = _map.spawnResources();
        _clock.resetSpawn();
        for (auto &tile : changedTiles) {
            int x = tile.first;
            int y = tile.second;
            notifyGui([x, y, this](GuiClient &gui) {
                gui.bct(x, y, _map);
            });
        }
    }

    int elapsedTicks = _clock.elapsedTicks();
    if (elapsedTicks <= 0)
        return;

    for (size_t i = 0; i < _clients.size(); i++) {
        auto player = std::dynamic_pointer_cast<Player>(_clients[i]);
        if (player == nullptr)
            continue;
        int eaten = player->decrementLife(elapsedTicks);
        if (eaten > 0) {
            notifyGui([&player](GuiClient &gui) {
                gui.pin(player);
            });
        }
        if (player->isDead()) {
            std::cout << "Player on fd " << player->getFd() << " has died" << std::endl;
            player->sendMessage("dead\n");
            notifyGui([&player](GuiClient &gui) {
                gui.pdi(player);
            });
            removeClient(i + 1);
            i--;
            continue;
        }
        processPlayerCommands(player, _clock, _map, _teams, _clients);
    }
}
