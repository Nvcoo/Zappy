/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Map
*/

#ifndef MAP_HPP_
    #define MAP_HPP_

#include "Tile.hpp"
#include <vector>

namespace world {

class Map {
    private:
        int _width;
        int _height;
        std::vector<Tile> _tiles;
        int wrapX(int x) const;
        int wrapY(int y) const;
        int toIndex(int x, int y) const;
        int computeDensity(Resource r) const;
    protected:
    public:
        Map(int width, int height);
        int getWidth() const;
        int getHeight() const;
        Tile &getTile(int x, int y);
        //we could add another &getTile function with a const for read-only purposes
        //but I see no point for now.
        void spawnResources();
};

}

#endif
