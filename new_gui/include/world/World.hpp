/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** World
*/

#ifndef WORLD_HPP_
    #define WORLD_HPP_

#include <string>
#include <unordered_map>
#include <vector>
#include "Tile.hpp"
#include "Player.hpp"

struct Egg {
    int id;
    int x;
    int y;
};

class World {
    private:
        int _width;
        int _height;
        std::vector<Tile> _tiles;
        std::unordered_map<int, Player> _players;
        std::unordered_map<int, Egg> _eggs;
        std::vector<std::string> _teams;
        int toIndex(int x, int y) const //converts 2D tiles into an 1D array to store them
        {
            return y * _width + x;
        }
    protected:
    public:
        World() : _width(0), _height(0) {}
        void setSize(int width, int height);
        int getWidth() const
        {
            return _width;
        }
        int getHeight() const
        {
            return _height;
        }
        Tile &getTile(int x, int y)
        {
            return _tiles[toIndex(x, y)];
        }
        void addTeam(const std::string &name)
        {
            _teams.push_back(name);
        }
        const std::vector<std::string> &getTeams() const
        {
            return _teams;
        }
        void removePlayer(int id)
        {
            _players.erase(id);
        }
        const std::unordered_map<int, Player> &getPlayers() const
        {
            return _players;
        }
        void addEgg(int id, int x, int y)
        {
            _eggs.erase(id);
            _eggs.emplace(id, Egg{id, x, y});
        }
        void removeEgg(int id)
        {
            _eggs.erase(id);
        }
        const std::unordered_map<int, Egg> &getEggs() const
        {
            return _eggs;
        }
        void addPlayer(int id, int x, int y, orientation_t o, int level, const std::string &team);
        void movePlayer(int id, int x, int y, orientation_t o);
        void setPlayerLevel(int id, int level);
};

#endif
