/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Args
*/

#include "../../include/world/Tile.hpp"

namespace world {

Tile::Tile()
{
    _playerCount = 0;
    _resources.fill(0);
}

int Tile::getResource(Resource r) const
{
    return _resources[r];
}

void Tile::addResource(Resource r, int amount)
{
    _resources[r] += amount;
}

void Tile::removeResource(Resource r, int amount)
{
    _resources[r] -= amount;
    if (_resources[r] < 0)
        _resources[r] = 0;
}

int Tile::getPlayerCount() const
{
    return _playerCount;
}

void Tile::addPlayer()
{
    _playerCount++;
}

void Tile::removePlayer()
{
    if (_playerCount > 0)
        _playerCount--;
}

}
