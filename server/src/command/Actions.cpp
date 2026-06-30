/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Actions
*/

#include "../../include/command/Actions.hpp"
#include <string>

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

void moveForward(game::Player &player, world::Map &map)
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
    response += "food " + std::to_string(player.getInventory(world::FOOD));
    response += ", linemate " + std::to_string(player.getInventory(world::LINEMATE));
    response += ", deraumere " + std::to_string(player.getInventory(world::DERAUMERE));
    response += ", sibur " + std::to_string(player.getInventory(world::SIBUR));
    response += ", mendiane " + std::to_string(player.getInventory(world::MENDIANE));
    response += ", phiras " + std::to_string(player.getInventory(world::PHIRAS));
    response += ", thystame " + std::to_string(player.getInventory(world::THYSTAME));
    response += "]\n";
    return response;
}

}
