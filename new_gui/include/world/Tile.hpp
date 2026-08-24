/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Tile
*/

#ifndef TILE_HPP_
    #define TILE_HPP_

#include <array>

enum Resource {
    FOOD,
    LINEMATE,
    DERAUMERE,
    SIBUR,
    MENDIANE,
    PHIRAS,
    THYSTAME,
    RESOURCE_COUNT
};

class Tile {
    private:
        std::array<int, RESOURCE_COUNT> _resources;
    protected:
    public:
        Tile()
        {
            _resources.fill(0);
        }
        int getResource(Resource r) const
        {
            return _resources[r];
        }
        void setResource(Resource r, int amount)
        {
            _resources[r] = amount;
        }
};

#endif
