/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Elevation
*/

#include "../../include/world/Elevation.hpp"

bool checkRequirements(int level, int playerCount, const Tile &tile)
{
    if (level < 1 || level > 7)
        return false;

    const reqs &required = table[level - 1];

    if (playerCount < required.players)
        return false;
    if (tile.getResource(LINEMATE) < required.linemate)
        return false;
    if (tile.getResource(DERAUMERE) < required.deraumere)
        return false;
    if (tile.getResource(SIBUR) < required.sibur)
        return false;
    if (tile.getResource(MENDIANE) < required.mendiane)
        return false;
    if (tile.getResource(PHIRAS) < required.phiras)
        return false;
    if (tile.getResource(THYSTAME) < required.thystame)
        return false;
    return true;
}

void consumeResources(int level, Tile &tile)
{
    if (level < 1 || level > 7)
        return;

    const reqs &required = table[level - 1];

    tile.removeResource(LINEMATE, required.linemate);
    tile.removeResource(DERAUMERE, required.deraumere);
    tile.removeResource(SIBUR, required.sibur);
    tile.removeResource(MENDIANE, required.mendiane);
    tile.removeResource(PHIRAS, required.phiras);
    tile.removeResource(THYSTAME, required.thystame);
}
