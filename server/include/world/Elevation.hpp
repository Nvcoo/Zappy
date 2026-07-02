/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Elevation
*/

#ifndef ELEVATION_HPP_
    #define ELEVATION_HPP_

#include "Tile.hpp"

struct reqs {
    int players;
    int linemate;
    int deraumere;
    int sibur;
    int mendiane;
    int phiras;
    int thystame;
};

static const reqs table[7] = {
    {1, 1, 0, 0, 0, 0, 0},
    {2, 1, 1, 1, 0, 0, 0},
    {2, 2, 0, 1, 0, 2, 0},
    {4, 1, 1, 2, 0, 1, 0},
    {4, 1, 2, 1, 3, 0, 0},
    {6, 1, 2, 3, 0, 1, 0},
    {6, 2, 2, 2, 2, 2, 1}
};

bool checkRequirements(int level, int playerCount, const Tile &tile);
void consumeResources(int level, Tile &tile);

#endif
