/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Inventory
*/

#include "../../../include/command/Actions.hpp"

std::string executeInventory(Player &player)
{
    std::string response = "[";
    response += "food " + std::to_string(player.getInventory(FOOD));
    response += ", linemate " + std::to_string(player.getInventory(LINEMATE));
    response += ", deraumere " + std::to_string(player.getInventory(DERAUMERE));
    response += ", sibur " + std::to_string(player.getInventory(SIBUR));
    response += ", mendiane " + std::to_string(player.getInventory(MENDIANE));
    response += ", phiras " + std::to_string(player.getInventory(PHIRAS));
    response += ", thystame " + std::to_string(player.getInventory(THYSTAME));
    response += "]\n";
    return response;
}

std::string executeTake(Player &player, Map &map, const std::string &arg)
{
    Resource r = nameToResource(arg);

    if (r == RESOURCE_COUNT)
        return "ko\n";

    Tile &tile = map.getTile(player.getX(), player.getY());
    if (tile.getResource(r) <= 0)
        return "ko\n";
    tile.removeResource(r, 1);
    player.addToInv(r, 1);
    return "ok\n";
}

std::string executeSet(Player &player, Map &map, const std::string &arg)
{
    Resource r = nameToResource(arg);

    if (r == RESOURCE_COUNT)
        return "ko\n";
    if (player.getInventory(r) <= 0)
        return "ko\n";
    player.removeFromInv(r, 1);
    Tile &tile = map.getTile(player.getX(), player.getY());
    tile.addResource(r, 1);
    return "ok\n";
}
