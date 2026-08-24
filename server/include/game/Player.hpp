/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Player
*/

#ifndef PLAYER_HPP_
    #define PLAYER_HPP_

#include <cstdlib>
#include <cstring>
#include <queue>
#include <string>
#include "../network/Client.hpp"
#include "../world/Tile.hpp"

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
    bool started;
};

class Player : public Client {
    private:
        int _x;
        int _y;
        orientation_t _orientation;
        int _level;
        int _inventory[RESOURCE_COUNT];
        int _lifeTimer;
        std::string _teamName;
        std::queue<Command> _commandQueue;
        bool _busy;
        bool _inIncantation;
        int _id;
        static int _nextId;
    protected:
    public:
        Player(int fd, const std::string &teamName, int x, int y) : Client(fd, PLAYER), _x(x), _y(y), _orientation(static_cast<orientation_t>(1 + std::rand() % 4)), _level(1), _lifeTimer(126), _teamName(teamName), _busy(false), _inIncantation(false), _id(_nextId++)
        {
            std::memset(_inventory, 0, sizeof(_inventory));
            _inventory[FOOD] = 10;
        }

        int getX() const
        {
            return _x;
        }
        int getY() const
        {
            return _y;
        }
        orientation_t getOrientation() const
        {
            return _orientation;
        }
        int getLevel() const
        {
            return _level;
        }
        int getInventory(Resource r) const
        {
            return _inventory[r];
        }
        int getLifeTimer() const
        {
            return _lifeTimer;
        }
        const std::string &getTeamName() const
        {
            return _teamName;
        }
        bool isBusy() const
        {
            return _busy;
        }
        void setPos(int x, int y)
        {
            _x = x;
            _y = y;
        }
        void setOrientation(orientation_t o)
        {
            _orientation = o;
        }
        void setLevel(int level)
        {
            _level = level;
        }
        void setBusy(bool busy)
        {
            _busy = busy;
        }
        bool isDead() const
        {
            return _lifeTimer <= 0;
        }
        bool isInIncantation() const
        {
            return _inIncantation;
        }
        void setInIncantation(bool v)
        {
            _inIncantation = v;
        }
        int getId() const
        {
            return _id;
        }
        void addToInv(Resource r, int amount);
        void removeFromInv(Resource r, int amount);
        int decrementLife(int amount);

        bool pushCommand(const Command &cmd);
        bool hasCommand() const;
        Command &frontCommand();
        void popCommand();
};

#endif
