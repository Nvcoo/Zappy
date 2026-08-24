/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** World
*/

#include "../../include/world/World.hpp"

void World::setSize(int width, int height)
{
    _width = width;
    _height = height;
    _tiles.assign(width * height, Tile());
}

void World::addPlayer(int id, int x, int y, orientation_t o, int level, const std::string &team)
{
    _players.erase(id);
    _players.emplace(id, Player(id, x, y, o, level, team)); //not the most perfect use of emplace, but it works.
}

void World::movePlayer(int id, int x, int y, orientation_t o)
{
    auto it = _players.find(id);

    if (it != _players.end())
        it->second.setPos(x, y, o);
}

void World::setPlayerLevel(int id, int level)
{
    auto it = _players.find(id);

    if (it != _players.end())
        it->second.setLevel(level);
}
