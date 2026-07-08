/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Movement
*/

#include "../../../include/command/Actions.hpp"
#include <cstddef>
#include <memory>
#include <string>
#include <utility>
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
