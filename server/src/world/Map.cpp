/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Args
*/

#include "../../include/world/Map.hpp"
#include <cstdlib>
#include <ctime>

namespace world {

Map::Map(int width, int height) : _width(width), _height(height)
{
    _tiles.resize(width * height);
    std::srand(std::time(nullptr)); //just a line that randomizes resource spawing
    spawnResources();
}

int Map::getWidth() const
{
    return _width;
}

int Map::getHeight() const
{
    return _height;
}

int Map::wrapX(int x) const
{
    return ((x % _width) + _width) % _width;
}

int Map::wrapY(int y) const
{
    return ((y % _height) + _height) % _height;
}

int Map::toIndex(int x, int y) const
{
    return wrapY(y) * _width + wrapX(x);
}

Tile &Map::getTile(int x, int y)
{
    return _tiles[toIndex(x, y)];
}

int Map::computeDensity(Resource r) const
{
    int target = static_cast<int>(_width * _height * densities[r]);
    return target < 1 ? 1 : target;
}

void Map::spawnResources()
{
    for (int i = 0; i < RESOURCE_COUNT; i++) {
        Resource r = static_cast<Resource>(i);
        int target = computeDensity(r);
        int current = 0;
        for (auto &tile : _tiles)
            current += tile.getResource(r);
        int toAdd = target - current;
        for (int j = 0; j < toAdd; j++) {
            int x = std::rand() % _width;
            int y = std::rand() % _height;
            getTile(x, y).addResource(r, 1);
        }
    }
}

}
