/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Actions
*/

#include "../../include/command/Actions.hpp"
#include <string>
#include <vector>

namespace command {

void rotateLeft(game::Player &player)
{
    switch (player.getOrientation()) {
        case game::NORTH:
            player.setOrientation(game::WEST);
            break;
        case game::WEST:
            player.setOrientation(game::SOUTH);
            break;
        case game::SOUTH:
            player.setOrientation(game::EAST);
            break;
        case game::EAST:
            player.setOrientation(game::NORTH);
            break;
    }
}

void rotateRight(game::Player &player)
{
    switch (player.getOrientation()) {
        case game::NORTH:
            player.setOrientation(game::EAST);
            break;
        case game::WEST:
            player.setOrientation(game::NORTH);
            break;
        case game::SOUTH:
            player.setOrientation(game::WEST);
            break;
        case game::EAST:
            player.setOrientation(game::SOUTH);
            break;
    }
}

void moveForward(game::Player &player, Map &map)
{
    int x = player.getX();
    int y = player.getY();

    switch (player.getOrientation()) {
        case game::NORTH:
            y -= 1;
            break;
        case game::SOUTH:
            y += 1;
            break;
        case game::EAST:
            x += 1;
            break;
        case game::WEST:
            x -= 1;
            break;
    }
    x = ((x % map.getWidth()) + map.getWidth()) % map.getWidth();
    y = ((y % map.getHeight()) + map.getHeight()) % map.getHeight();
    player.setPos(x, y);
}

std::string executeInventory(game::Player &player)
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

std::string executeTake(game::Player &player, Map &map, const std::string &arg)
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

std::string executeSet(game::Player &player, Map &map, const std::string &arg)
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

std::string executeConnectNbr(game::Player &player, std::vector<Team> &teams)
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

}
