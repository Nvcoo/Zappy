/*
** EPITECH PROJECT, 2026
** Zappy
** File description:
** Player
*/

#ifndef PLAYER_HPP_
    #define PLAYER_HPP_

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
        Player(int fd, const std::string &teamName, int x, int y);

        int getX() const;
        int getY() const;
        orientation_t getOrientation() const;
        int getLevel() const;
        int getInventory(world::Resource r) const;
        int getLifeTimer() const;
        const std::string &getTeamName() const;
        bool isBusy() const;

        void setPos(int x, int y);
        void setOrientation(orientation_t o);
        void setLevel(int level);
        void addToInv(world::Resource r, int amount);
        void removeFromInv(world::Resource r, int amount);
        void decrementLife(int amount);
        bool isDead() const;

        bool pushCommand(const Command &cmd);
        bool hasCommand() const;
        Command &frontCommand();
        void popCommand();
        void setBusy(bool busy);
};

}

#endif
