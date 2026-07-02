/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Actions
*/

#include "../../include/command/Actions.hpp"
#include <string>
#include <vector>

void rotateLeft(Player &player)
{
    switch (player.getOrientation()) {
        case NORTH:
            player.setOrientation(WEST);
            break;
        case WEST:
            player.setOrientation(SOUTH);
            break;
        case SOUTH:
            player.setOrientation(EAST);
            break;
        case EAST:
            player.setOrientation(NORTH);
            break;
    }
}

void rotateRight(Player &player)
{
    switch (player.getOrientation()) {
        case NORTH:
            player.setOrientation(EAST);
            break;
        case WEST:
            player.setOrientation(NORTH);
            break;
        case SOUTH:
            player.setOrientation(WEST);
            break;
        case EAST:
            player.setOrientation(SOUTH);
            break;
    }
}

void moveForward(Player &player, Map &map)
{
    int x = player.getX();
    int y = player.getY();

    switch (player.getOrientation()) {
        case NORTH:
            y -= 1;
            break;
        case SOUTH:
            y += 1;
            break;
        case EAST:
            x += 1;
            break;
        case WEST:
            x -= 1;
            break;
    }
    x = ((x % map.getWidth()) + map.getWidth()) % map.getWidth();
    y = ((y % map.getHeight()) + map.getHeight()) % map.getHeight();
    player.setPos(x, y);
}

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
    player.removeFromInv(r, r);
    Tile &tile = map.getTile(player.getX(), player.getY());
    tile.addResource(r, r);
    return "ok\n";
}

std::string executeConnectNbr(Player &player, std::vector<Team> &teams)
{
    for (auto &team: teams) {
        if (team.getName() == player.getTeamName())
            return std::to_string(team.getAvailableSlots()) + "\n";
    }
    return "0\n";
}

Resource nameToResource(const std::string &name)
{
    if (name == "food")
        return FOOD;
    if (name == "linemate")
        return LINEMATE;
    if (name == "deraumere")
        return DERAUMERE;
    if (name == "sibur")
        return SIBUR;
    if (name == "mendiane")
        return MENDIANE;
    if (name == "phiras")
        return PHIRAS;
    if (name == "thystame")
        return THYSTAME;
    return RESOURCE_COUNT; //we'll return this if it's invalid
}
