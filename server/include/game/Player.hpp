/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Player
*/

#ifndef PLAYER_HPP_
    #define PLAYER_HPP_

#include <cstring>
#include <queue>
#include <string>
#include "../network/Client.hpp"
#include "../world/Tile.hpp"

namespace game {

typedef enum orientation {
    NORTH = 1,
    EAST = 2,
    SOUTH = 3,
    WEST = 4
} orientation_t;

struct Command {
    std::string name;
    std::string arg;
    int executeAt;
};

class Player : public network::Client {
    private:
        int _x;
        int _y;
        orientation_t _orientation;
        int _level;
        int _inventory[world::RESOURCE_COUNT];
        int _lifeTimer;
        std::string _teamName;
        std::queue<Command> _commandQueue;
        bool _busy;
    protected:
    public:
        Player(int fd, const std::string &teamName, int x, int y) : network::Client(fd, network::PLAYER), _x(x), _y(y), _orientation(NORTH), _level(1), _lifeTimer(1260), _teamName(teamName), _busy(false)
        {
            std::memset(_inventory, 0, sizeof(_inventory));
            _inventory[world::FOOD] = 10;
        };

        int getX() const
        {
            return _x;
        };
        int getY() const
        {
            return _y;
        };
        orientation_t getOrientation() const
        {
            return _orientation;
        };
        int getLevel() const
        {
            return _level;
        };
        int getInventory(world::Resource r) const
        {
            return _inventory[r];
        };
        int getLifeTimer() const
        {
            return _lifeTimer;
        };
        const std::string &getTeamName() const
        {
            return _teamName;
        };
        bool isBusy() const
        {
            return _busy;
        };

        void setPos(int x, int y)
        {
            _x = x;
            _y = y;
        };
        void setOrientation(orientation_t o)
        {
            _orientation = o;
        };
        void setLevel(int level)
        {
            _level = level;
        };
        void setBusy(bool busy)
        {
            _busy = busy;
        };
        bool isDead() const
        {
            return _lifeTimer <= 0;
        };
        void addToInv(world::Resource r, int amount);
        void removeFromInv(world::Resource r, int amount);
        void decrementLife(int amount);

        bool pushCommand(const Command &cmd);
        bool hasCommand() const;
        Command &frontCommand();
        void popCommand();
};

}

#endif
