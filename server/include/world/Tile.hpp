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
    RESOURCE_COUNT //Serves no purpose appart from making the resource array clearer
};

class Tile {
    private:
        std::array<int, RESOURCE_COUNT> _resources;
    protected:
    public:
        Tile();
        int getResource(Resource r) const;
        void addResource(Resource r, int amount);
        void removeResource(Resource r, int amount);
};

#endif
